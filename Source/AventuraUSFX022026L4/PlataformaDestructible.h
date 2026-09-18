// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PlataformaDestructible.generated.h"

// Hereda de APlataforma. La pelota la destruye, el Pawn rebota.
UCLASS()
class AVENTURAUSFX022026L4_API APlataformaDestructible : public APlataforma
{
    GENERATED_BODY()

public:
    APlataformaDestructible();
};