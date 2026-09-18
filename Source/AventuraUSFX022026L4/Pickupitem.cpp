#include "Pickupitem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

APickupitem::APickupitem()
{
    PrimaryActorTick.bCanEverTick = true;

    // Malla
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Game/SciFi_Props/Models/SM_Box_3.SM_Box_3'"));
    if (MeshAsset.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshAsset.Object);
        MeshComp->SetRelativeScale3D(FVector(1.5f));
        MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // Esfera de colisión (Overlap)
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SphereComp->SetupAttachment(RootComponent);
    SphereComp->InitSphereRadius(60.0f);

    SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComp->SetCollisionObjectType(ECC_GameTraceChannel1);
    SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComp->SetGenerateOverlapEvents(true);

    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickupitem::OnOverlapBegin);
    SphereComp->OnComponentEndOverlap.AddDynamic(this, &APickupitem::OnOverlapEnd);

    VelocidadRotacion = 90.0f;
    bSiendoRecolectado = false;
}

void APickupitem::BeginPlay()
{
    Super::BeginPlay();
}
//funcion para que gire en su propio eje y se recoja al tocarlo
void APickupitem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float RotacionActual = bSiendoRecolectado ? (VelocidadRotacion * 4.0f) : VelocidadRotacion;
    AddActorLocalRotation(FRotator(0.0f, RotacionActual * DeltaTime, 0.0f));
}

void APickupitem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !OtherActor->IsA(APawn::StaticClass())) return;

    bSiendoRecolectado = true;
    MeshComp->SetRelativeScale3D(FVector(0.8f));

    GetWorldTimerManager().SetTimer(
        TimerRecoleccion,
        this,
        &APickupitem::CompletarRecoleccion,
        1.0f,
        false
    );
}

void APickupitem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || !OtherActor->IsA(APawn::StaticClass())) return;

    bSiendoRecolectado = false;
    MeshComp->SetRelativeScale3D(FVector(0.5f));

    GetWorldTimerManager().ClearTimer(TimerRecoleccion);
}

void APickupitem::CompletarRecoleccion()
{
    if (bSiendoRecolectado)
    {
        Destroy();
    }
}