// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L4GameMode.h"
#include "AventuraUSFX022026L4Pawn.h"
#include "PlataformaIndestructible.h"
#include "PlataformaDestructible.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AAventuraUSFX022026L4GameMode::AAventuraUSFX022026L4GameMode()
{
    DefaultPawnClass = AAventuraUSFX022026L4Pawn::StaticClass();

    // Cargar la malla del cubo en el constructor (FObjectFinder solo funciona aquí)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (CubeMesh.Succeeded())
    {
        CubeMeshReference = CubeMesh.Object;
    }
    else
    {
        CubeMeshReference = nullptr;
    }
}

void AAventuraUSFX022026L4GameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!CubeMeshReference)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
            TEXT("ERROR: CubeMeshReference es NULL"));
        return;
    }

    ConstruirEscenario();
    SpawnearEnemigo();
    IniciarSpawnTimer();
}

void AAventuraUSFX022026L4GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAventuraUSFX022026L4GameMode::ConstruirEscenario()
{
    // ==================== SUELO ====================
    // Suelo 3000 x 2000 centrado en el origen
    SpawnearPared(FVector(0.f, 0.f, 0.f), FVector(30.f, 20.f, 0.5f), FLinearColor(0.3f, 0.3f, 0.3f));

    // ==================== PAREDES EXTERNAS (400 unidades de alto) ====================
    // Pared derecha (X = +1500)
    SpawnearPared(FVector(1500.f, 0.f, 200.f), FVector(0.5f, 20.f, 4.f), FLinearColor(0.5f, 0.5f, 0.5f));
    // Pared izquierda (X = -1500)
    SpawnearPared(FVector(-1500.f, 0.f, 200.f), FVector(0.5f, 20.f, 4.f), FLinearColor(0.5f, 0.5f, 0.5f));
    // Pared frontal (Y = +1000)
    SpawnearPared(FVector(0.f, 1000.f, 200.f), FVector(30.f, 0.5f, 4.f), FLinearColor(0.5f, 0.5f, 0.5f));
    // Pared trasera (Y = -1000)
    SpawnearPared(FVector(0.f, -1000.f, 200.f), FVector(30.f, 0.5f, 4.f), FLinearColor(0.5f, 0.5f, 0.5f));

    // ==================== 2 MUROS INTERNOS VERTICALES (eje Y) ====================
    // Van a lo largo del eje Y, dividen el escenario en 3 secciones (izq, centro, der)
    // Son más cortos que los muros izquierdo/derecho: solo 1200 unidades de largo (de -600 a +600)
    // Dejan un hueco arriba y abajo para que el Pawn pase por debajo sin colisionar
    SpawnearPared(FVector(-500.f, 0.f, 200.f), FVector(0.5f, 12.f, 4.f), FLinearColor(0.9f, 0.9f, 0.2f));
    SpawnearPared(FVector(500.f, 0.f, 200.f), FVector(0.5f, 12.f, 4.f), FLinearColor(0.9f, 0.9f, 0.2f));
}

void AAventuraUSFX022026L4GameMode::SpawnearPared(FVector Posicion, FVector Escala, FLinearColor Color)
{
    UWorld* World = GetWorld();
    if (!World) return;
    if (!CubeMeshReference) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AStaticMeshActor* Pared = World->SpawnActor<AStaticMeshActor>(
        AStaticMeshActor::StaticClass(), Posicion, FRotator::ZeroRotator, Params);

    if (Pared)
    {
        UStaticMeshComponent* Mesh = Pared->GetStaticMeshComponent();
        if (Mesh)
        {
            // SetMobility ANTES de SetStaticMesh (obligatorio en runtime)
            Mesh->SetMobility(EComponentMobility::Movable);
            Mesh->SetStaticMesh(CubeMeshReference);
            Mesh->SetWorldScale3D(Escala);
            Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            Mesh->SetCollisionResponseToAllChannels(ECR_Block);
            Mesh->SetNotifyRigidBodyCollision(true);

            UMaterialInstanceDynamic* DynMat = Mesh->CreateDynamicMaterialInstance(0);
            if (DynMat)
            {
                DynMat->SetVectorParameterValue(FName("Color"), Color);
            }
        }
    }
}

void AAventuraUSFX022026L4GameMode::SpawnearEnemigo()
{
    UWorld* World = GetWorld();
    if (!World) return;
    if (!CubeMeshReference) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Enemigo en la sección central
    AStaticMeshActor* Enemigo = World->SpawnActor<AStaticMeshActor>(
        AStaticMeshActor::StaticClass(), FVector(0.f, 0.f, 200.f), FRotator::ZeroRotator, Params);

    if (Enemigo)
    {
        UStaticMeshComponent* Mesh = Enemigo->GetStaticMeshComponent();
        if (Mesh)
        {
            Mesh->SetMobility(EComponentMobility::Movable);
            Mesh->SetStaticMesh(CubeMeshReference);
            Mesh->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
            Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            Mesh->SetCollisionResponseToAllChannels(ECR_Block);
        }
    }
}

void AAventuraUSFX022026L4GameMode::IniciarSpawnTimer()
{
    // Spawn cada 5 segundos en bucle
    GetWorldTimerManager().SetTimer(
        TimerSpawnPlataforma,
        this,
        &AAventuraUSFX022026L4GameMode::SpawnearPlataformaAleatoria,
        5.0f,
        true
    );
}

void AAventuraUSFX022026L4GameMode::SpawnearPlataformaAleatoria()
{
    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Elegir una de las 3 secciones (izquierda, centro, derecha)
    int32 Seccion = FMath::RandRange(0, 2);

    float PosX = 0.0f;
    switch (Seccion)
    {
    case 0: PosX = FMath::FRandRange(-1400.f, -600.f); break;  // Sección izquierda
    case 1: PosX = FMath::FRandRange(-400.f, 400.f);   break;  // Sección central
    case 2: PosX = FMath::FRandRange(600.f, 1400.f);   break;  // Sección derecha
    }

    // Y aleatorio en todo el alto
    float PosY = FMath::FRandRange(-900.f, 900.f);

    // Z: parte superior del escenario
    float PosZ = 450.0f;

    FVector Posicion(PosX, PosY, PosZ);

    // Tipo aleatorio
    bool bIndestructible = FMath::RandBool();

    if (bIndestructible)
    {
        World->SpawnActor<APlataformaIndestructible>(
            APlataformaIndestructible::StaticClass(), Posicion, FRotator::ZeroRotator, Params);
    }
    else
    {
        World->SpawnActor<APlataformaDestructible>(
            APlataformaDestructible::StaticClass(), Posicion, FRotator::ZeroRotator, Params);
    }
}