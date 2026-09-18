// Fill out your copyright notice in the Description page of Project Settings.

#include "Plataforma.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

APlataforma::APlataforma()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));

    mallaPlataforma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPlataforma"));
    if (PlataformaMeshAsset.Succeeded())
    {
        mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);
    }
    RootComponent = mallaPlataforma;

    // Colisión Block: la pelota rebotará contra la plataforma
    mallaPlataforma->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    mallaPlataforma->SetCollisionResponseToAllChannels(ECR_Block);
    mallaPlataforma->SetNotifyRigidBodyCollision(true);

    ColorPlataforma = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
    movimientoDireccion = FVector(1.0f, 0.0f, 0.0f);
}

void APlataforma::BeginPlay()
{
    Super::BeginPlay();

    // Aplicar el color con un material dinámico (necesario para cambiar color en runtime)
    UMaterialInstanceDynamic* DynMat = mallaPlataforma->CreateDynamicMaterialInstance(0);
    if (DynMat)
    {
        DynMat->SetVectorParameterValue(FName("Color"), ColorPlataforma);
        DynMat->SetVectorParameterValue(FName("BaseColor"), ColorPlataforma);
    }
}

void APlataforma::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Caída: la plataforma baja hasta llegar al suelo
    if (!bEnElSuelo)
    {
        FVector Pos = GetActorLocation();
        Pos.Z -= VelocidadCaida * DeltaTime;

        if (Pos.Z <= AlturaSuelo)
        {
            Pos.Z = AlturaSuelo;
            bEnElSuelo = true;
        }
        SetActorLocation(Pos);
    }
}