#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickupitem.generated.h"

UCLASS()
class AVENTURAUSFX022026L4_API APickupitem : public AActor
{
    GENERATED_BODY()

public:
    APickupitem();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Malla cúbica (visual)
    class UStaticMeshComponent* MeshComp;

    // Esfera de colisión (Overlap)
    class USphereComponent* SphereComp;

    // Evento Overlap BEGIN
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    // Evento Overlap END
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    // Función del timer de recolección
    void CompletarRecoleccion();

private:
    float VelocidadRotacion;
    FTimerHandle TimerRecoleccion;
    bool bSiendoRecolectado;
};