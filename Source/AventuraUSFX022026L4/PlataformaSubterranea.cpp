// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaSubterranea.h"

APlataformaSubterranea::APlataformaSubterranea()
{
	PrimaryActorTick.bCanEverTick = true;

	TipoPlataforma = ETipoPlataforma::PLATAFORMA_SUBTERRANEA;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);

	movimientoEjes = FVector(1.0f, 0.0f, 1.0f);
	movimientoLimitesMaximos = FVector(500.0f, 0.0f, 500.0f);
	movimientoLimitesMinimos = FVector(0.0f, 0.0f, 200.0f);
	movimientoVelocidades = FVector(300.0f, 0.0f, 300.0f);
	movimientoDireccion = FVector(1.0f, 0.0f, 1.0f);

}
