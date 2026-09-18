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
    mallaPlataforma->SetRelativeScale3D(FVector(3.0f, 1.0f, 0.2f));

    // Color VERDE
    ColorPlataforma = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
}