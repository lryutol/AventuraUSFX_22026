// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AventuraUSFX022026L4GameMode.generated.h"

class UStaticMesh;

UCLASS(MinimalAPI)
class AAventuraUSFX022026L4GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAventuraUSFX022026L4GameMode();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void ConstruirEscenario();
    void SpawnearPared(FVector Posicion, FVector Escala, FLinearColor Color);
    void SpawnearEnemigo();
    void SpawnearPlataformaAleatoria();
    void IniciarSpawnTimer();

    FTimerHandle TimerSpawnPlataforma;

private:
    // Referencia a la malla de cubo cargada en el constructor
    UStaticMesh* CubeMeshReference;
};