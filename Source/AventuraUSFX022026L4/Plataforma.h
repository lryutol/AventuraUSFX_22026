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

    bool bEnElSuelo = false;
    float VelocidadCaida = 200.0f;
    float AlturaSuelo = 50.0f;   // Ajustado: suelo está en Z=25, plataforma de 40 de alto → centro en 50
};