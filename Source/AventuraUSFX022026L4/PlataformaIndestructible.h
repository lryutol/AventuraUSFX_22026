// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PlataformaIndestructible.generated.h"

// Hereda de APlataforma. La pelota rebota en ella, el Pawn la destruye.
UCLASS()
class AVENTURAUSFX022026L4_API APlataformaIndestructible : public APlataforma
{
    GENERATED_BODY()

public:
    APlataformaIndestructible();
};