// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AventuraUSFX022026L4Pawn.generated.h"

UCLASS(Blueprintable)
class AAventuraUSFX022026L4Pawn : public APawn
{
    GENERATED_BODY()

    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ShipMeshComponent;

    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* CameraComponent;

    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

public:
    AAventuraUSFX022026L4Pawn();

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector GunOffset;

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float FireRate;

    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

    UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
    class USoundBase* FireSound;

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void FireShot();
    void ShotTimerExpired();

    static const FName MoveRightBinding;

private:
    uint32 bCanFire : 1;
    FTimerHandle TimerHandle_ShotTimerExpired;

protected:
    // Evento Hit: destruye PlataformaIndestructible al chocar
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

public:
    FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
    FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};