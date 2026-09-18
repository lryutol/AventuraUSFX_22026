// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaTerrestre.h"

APlataformaTerrestre::APlataformaTerrestre()
{
	PrimaryActorTick.bCanEverTick = true;

	TipoPlataforma = ETipoPlataforma::PLATAFORMA_TERRESTRE;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);

	movimientoEjes = FVector(0.0f, 1.0f, 0.0f);
	movimientoLimitesMaximos = FVector(0.0f, 500.0f, 0.0f);
	movimientoLimitesMinimos = FVector(0.0f, 0.0f, 0.0f);
	movimientoVelocidades = FVector(0.0f, 1000.0f, 0.0f);
	movimientoDireccion = FVector(0.0f, 1.0f, 0.0f);
}
