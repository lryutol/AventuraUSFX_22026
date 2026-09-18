#include "BouncingCube.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"

ABouncingCube::ABouncingCube()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (MeshAsset.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshAsset.Object);
        MeshComp->SetRelativeScale3D(FVector(2.5f));
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(
        TEXT("Material'/Game/TwinStick/Meshes/material_1.material_1'"));
    if (MaterialAsset.Succeeded())
    {
        MeshComp->SetMaterial(0, MaterialAsset.Object);
    }

    // Colisión Block
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionObjectType(ECC_GameTraceChannel2);
    MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
    //MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // ← Ignora al Pawn

    // Física: obligatoria para que OnHit se dispare
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetEnableGravity(false);
    MeshComp->SetNotifyRigidBodyCollision(true);

    MeshComp->OnComponentHit.AddDynamic(this, &ABouncingCube::OnHit);

    bYaSeMultiplico = false;
    EsferasPorMultiplicacion = 4;
    NivelActual = 0;
    NivelMaximo = 3;
    FactorEscala = 0.5f;
    RadioDispersion = 150.0f;
}
// EVENTO HIT (BLOCK)
// Se ejecuta automáticamente cuando algo choca contra la esfera.
void ABouncingCube::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    // Filtros: verificar si realmente debemos reaccionar
    if (!OtherActor || OtherActor == this) return;
    if (!OtherActor->IsA(APawn::StaticClass())) return;
    if (bYaSeMultiplico) return;
    if (NivelActual >= NivelMaximo) return;

    bYaSeMultiplico = true;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector UbicacionActual = GetActorLocation();
    FVector EscalaNueva = GetActorScale3D() * FactorEscala;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//funsion para crear las esferas hijas en un circulo alrededor de la esfera original
    for (int32 i = 0; i < EsferasPorMultiplicacion; i++)
    {
        float Angulo = (2.0f * PI * i) / EsferasPorMultiplicacion;
        FVector Offset(FMath::Cos(Angulo) * RadioDispersion, FMath::Sin(Angulo) * RadioDispersion, 0.0f);
        //float OffsetX = (i - (EsferasPorMultiplicacion - 1) * 0.5f) * RadioDispersion;
        //FVector Offset(OffsetX, 0.0f, 0.0f);

        ABouncingCube* Nueva = World->SpawnActor<ABouncingCube>(
            ABouncingCube::StaticClass(), UbicacionActual + Offset, FRotator::ZeroRotator, Params);

        if (Nueva)
        {
            Nueva->SetActorScale3D(EscalaNueva);
            Nueva->NivelActual = NivelActual + 1;
            Nueva->NivelMaximo = NivelMaximo;
            Nueva->EsferasPorMultiplicacion = EsferasPorMultiplicacion;
            Nueva->FactorEscala = FactorEscala;
            Nueva->RadioDispersion = RadioDispersion;
        }
    }

    Destroy();
}