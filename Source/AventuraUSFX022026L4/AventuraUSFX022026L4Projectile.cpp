// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L4Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "PlataformaDestructible.h"

AAventuraUSFX022026L4Projectile::AAventuraUSFX022026L4Projectile()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
    if (ProjectileMeshAsset.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
        ProjectileMesh->SetRelativeScale3D(FVector(0.5f));
    }
    RootComponent = ProjectileMesh;

    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Block);
    ProjectileMesh->SetNotifyRigidBodyCollision(true);

    ProjectileMesh->OnComponentHit.AddDynamic(this, &AAventuraUSFX022026L4Projectile::OnHit);

    // ProjectileMovement: la pelota rebota indefinidamente sin gravedad ni fricción
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 1200.f;
    ProjectileMovement->MaxSpeed = 1200.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;             // Rebota
    ProjectileMovement->Bounciness = 1.0f;                // Sin pérdida de energía
    ProjectileMovement->Friction = 0.0f;                  // Sin fricción
    ProjectileMovement->ProjectileGravityScale = 0.0f;    // Sin gravedad

    InitialLifeSpan = 0.0f;  // No se destruye por tiempo
}

void AAventuraUSFX022026L4Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    // La pelota destruye solo a las PlataformaDestructible
    if (OtherActor->IsA(APlataformaDestructible::StaticClass()))
    {
        OtherActor->Destroy();
    }
}