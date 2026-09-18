// Fill out your copyright notice in the Description page of Project Settings.

#include "PlataformaIndestructible.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

APlataformaIndestructible::APlataformaIndestructible()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded())
    {
        mallaPlataforma->SetStaticMesh(MeshAsset.Object);
    }

    // Plataformas anchas y alargadas
    mallaPlataforma->SetRelativeScale3D(FVector(9.0f, 2.5f, 0.4f));

    // Color VERDE
    ColorPlataforma = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
}