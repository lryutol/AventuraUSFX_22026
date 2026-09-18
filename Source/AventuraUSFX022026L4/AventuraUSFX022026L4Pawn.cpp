// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L4Pawn.h"
#include "AventuraUSFX022026L4Projectile.h"
#include "PlataformaIndestructible.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName AAventuraUSFX022026L4Pawn::MoveRightBinding("MoveRight");

AAventuraUSFX022026L4Pawn::AAventuraUSFX022026L4Pawn()
{
    // Malla de plataforma (plano)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PawnMesh(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
    ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    RootComponent = ShipMeshComponent;
    if (PawnMesh.Succeeded())
    {
        ShipMeshComponent->SetStaticMesh(PawnMesh.Object);
        ShipMeshComponent->SetRelativeScale3D(FVector(2.0f, 1.0f, 0.2f));
    }

    ShipMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ShipMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    ShipMeshComponent->SetNotifyRigidBodyCollision(true);

    ShipMeshComponent->OnComponentHit.AddDynamic(this, &AAventuraUSFX022026L4Pawn::OnHit);

    // ==================== CÁMARA ====================
    // Pitch = -85 (mirando casi vertical hacia abajo)
    // Yaw = 90 (mirando hacia +Y, así el eje X aparece como izq/der en pantalla)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->TargetArmLength = 2600.f;
    CameraBoom->SetRelativeRotation(FRotator(-85.f, 90.f, 0.f));
    CameraBoom->bDoCollisionTest = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->SetFieldOfView(90.f);

    // ==================== MOVIMIENTO ====================
    MoveSpeed = 800.0f;
    GunOffset = FVector(0.f, 0.f, 100.f);
    FireRate = 0.3f;
    bCanFire = true;

    // Pawn fijo en la parte baja del escenario
    FixedYPosition = -700.0f;
    MinXLimit = -1400.0f;
    MaxXLimit = 1400.0f;
}

void AAventuraUSFX022026L4Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    // Solo eje MoveRight (A / D)
    PlayerInputComponent->BindAxis(MoveRightBinding);

    // Disparo con Fire
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAventuraUSFX022026L4Pawn::FireShot);
}

void AAventuraUSFX022026L4Pawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const float RightValue = GetInputAxisValue(MoveRightBinding);

    // Movimiento SOLO en el eje X (izquierda/derecha en el mundo)
    FVector CurrentPos = GetActorLocation();
    float NewX = CurrentPos.X + (RightValue * MoveSpeed * DeltaSeconds);

    // Aplicar límites
    NewX = FMath::Clamp(NewX, MinXLimit, MaxXLimit);

    // Forzar Y fija y aplicar nueva X
    FVector NewPos = FVector(NewX, FixedYPosition, CurrentPos.Z);
    SetActorLocation(NewPos);
}

void AAventuraUSFX022026L4Pawn::FireShot()
{
    if (!bCanFire) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 150.f);

    // Dirección horizontal con yaw aleatorio
    float RandomYaw = FMath::FRandRange(0.0f, 360.0f);
    FRotator SpawnRotation = FRotator(0.f, RandomYaw, 0.f);

    World->SpawnActor<AAventuraUSFX022026L4Projectile>(
        AAventuraUSFX022026L4Projectile::StaticClass(), SpawnLocation, SpawnRotation);

    bCanFire = false;
    World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this,
        &AAventuraUSFX022026L4Pawn::ShotTimerExpired, FireRate);

    if (FireSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}

void AAventuraUSFX022026L4Pawn::ShotTimerExpired()
{
    bCanFire = true;
}

void AAventuraUSFX022026L4Pawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    // El Pawn destruye solo a las PlataformaIndestructible
    if (OtherActor->IsA(APlataformaIndestructible::StaticClass()))
    {
        OtherActor->Destroy();
    }
}