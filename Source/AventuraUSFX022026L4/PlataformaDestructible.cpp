// Fill out your copyright notice in the Description page of Project Settings.

#include "PlataformaDestructible.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

APlataformaDestructible::APlataformaDestructible()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
    if (MeshAsset.Succeeded())
    {
        mallaPlataforma->SetStaticMesh(MeshAsset.Object);
    }

    // Misma escala que la Indestructible
    mallaPlataforma->SetRelativeScale3D(FVector(9.0f, 2.5f, 0.4f));

    // Color ROJO
    ColorPlataforma = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
}