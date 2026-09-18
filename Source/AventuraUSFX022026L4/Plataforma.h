// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plataforma.generated.h"

class UStaticMeshComponent;

enum class ETipoPlataforma
{
    PLATAFORMA_AEREA,
    PLATAFORMA_TERRESTRE,
    PLATAFORMA_SUBTERRANEA,
    PLATAFORMA_ACUATICA
};

UCLASS()
class AVENTURAUSFX022026L4_API APlataforma : public AActor
{
    GENERATED_BODY()

public:
    ETipoPlataforma TipoPlataforma;
    APlataforma();

    UStaticMeshComponent* mallaPlataforma;

    // Color que se aplicará en BeginPlay para diferenciar las plataformas
    FLinearColor ColorPlataforma;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    FVector movimientoEjes = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoLimitesMaximos = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoLimitesMinimos = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoVelocidades = FVector(0.0f, 0.0f, 0.0f);
    FVector movimientoDireccion = FVector(0.0f, 0.0f, 0.0f);
    FVector posicionActual = FVector(0.0f, 0.0f, 0.0f);

    // Variables para la caída de las plataformas
    bool bEnElSuelo = false;
    float VelocidadCaida = 200.0f;
    float AlturaSuelo = 30.0f;
};