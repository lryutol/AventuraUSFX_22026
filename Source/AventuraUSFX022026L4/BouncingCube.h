#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BouncingCube.generated.h"

UCLASS()
class AVENTURAUSFX022026L4_API ABouncingCube : public AActor
{
    GENERATED_BODY()

public:
    ABouncingCube();

protected:
    class UStaticMeshComponent* MeshComp;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);

private:
    bool bYaSeMultiplico;
    int32 EsferasPorMultiplicacion;
    int32 NivelActual;
    int32 NivelMaximo;
    float FactorEscala;
    float RadioDispersion;
};