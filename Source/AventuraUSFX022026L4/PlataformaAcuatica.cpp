// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaAcuatica.h"

APlataformaAcuatica::APlataformaAcuatica()
{
	PrimaryActorTick.bCanEverTick = true;
	TipoPlataforma = ETipoPlataforma::PLATAFORMA_ACUATICA;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);

	movimientoEjes = FVector(1.0f, 0.0f, 0.0f);
	movimientoLimitesMaximos = FVector(2000.0f, 0.0f, 0.0f);
	movimientoLimitesMinimos = FVector(0.0f, 0.0f, 0.0f);
	movimientoVelocidades = FVector(200.0f, 0.0f, 0.0f);
	movimientoDireccion = FVector(1.0f, 0.0f, 0.0f);
}
