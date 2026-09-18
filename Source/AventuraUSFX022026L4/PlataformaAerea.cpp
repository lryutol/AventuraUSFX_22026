// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaAerea.h"

APlataformaAerea::APlataformaAerea()
{
	PrimaryActorTick.bCanEverTick = true;

	TipoPlataforma = ETipoPlataforma::PLATAFORMA_AEREA;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
	mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);

	movimientoEjes = FVector(0.0f, 0.0f, 1.0f);
	movimientoLimitesMaximos = FVector(0.0f, 0.0f, 800.0f);
	movimientoLimitesMinimos = FVector(0.0f, 0.0f, 0.0f);
	movimientoVelocidades = FVector(0.0f, 0.0f, 300.0f);
	movimientoDireccion = FVector(0.0f, 0.0f, 1.0f);
}
