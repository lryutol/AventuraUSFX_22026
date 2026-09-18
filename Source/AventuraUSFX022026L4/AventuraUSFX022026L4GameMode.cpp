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
#include "Kismet/GameplayStatics.h"

AAventuraUSFX022026L4GameMode::AAventuraUSFX022026L4GameMode()
{
    DefaultPawnClass = AAventuraUSFX022026L4Pawn::StaticClass();

    // Cargar la malla de cubo en el constructor (FObjectFinder solo funciona aquí)
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
    // Suelo
    SpawnearPared(FVector(0.f, 0.f, 0.f), FVector(30.f, 20.f, 0.5f), FLinearColor(0.3f, 0.3f, 0.3f));

    // 4 paredes externas
    SpawnearPared(FVector(1500.f, 0.f, 250.f), FVector(0.5f, 20.f, 5.f), FLinearColor(0.5f, 0.5f, 0.5f));
    SpawnearPared(FVector(-1500.f, 0.f, 250.f), FVector(0.5f, 20.f, 5.f), FLinearColor(0.5f, 0.5f, 0.5f));
    SpawnearPared(FVector(0.f, 1000.f, 250.f), FVector(30.f, 0.5f, 5.f), FLinearColor(0.5f, 0.5f, 0.5f));
    SpawnearPared(FVector(0.f, -1000.f, 250.f), FVector(30.f, 0.5f, 5.f), FLinearColor(0.5f, 0.5f, 0.5f));

    // Techo
    SpawnearPared(FVector(0.f, 0.f, 500.f), FVector(30.f, 20.f, 0.5f), FLinearColor(0.3f, 0.3f, 0.3f));

    // 2 paredes internas → dividen el escenario en 3 áreas
    SpawnearPared(FVector(-500.f, 0.f, 250.f), FVector(0.5f, 20.f, 5.f), FLinearColor(0.7f, 0.7f, 0.7f));
    SpawnearPared(FVector(500.f, 0.f, 250.f), FVector(0.5f, 20.f, 5.f), FLinearColor(0.7f, 0.7f, 0.7f));
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
    // Spawn cada 3 segundos en bucle
    GetWorldTimerManager().SetTimer(
        TimerSpawnPlataforma,
        this,
        &AAventuraUSFX022026L4GameMode::SpawnearPlataformaAleatoria,
        3.0f,
        true
    );
}

void AAventuraUSFX022026L4GameMode::SpawnearPlataformaAleatoria()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Obtener el Pawn del jugador para spawnear cerca de él
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(World, 0);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawnear 2 plataformas por oleada (para que sea más desafiante)
    int32 CantidadPorOleada = 2;

    for (int32 i = 0; i < CantidadPorOleada; i++)
    {
        float PosX = 0.0f;
        float PosY = 0.0f;

        if (Jugador)
        {
            // La plataforma cae cerca del jugador
            FVector PosJugador = Jugador->GetActorLocation();
            PosX = PosJugador.X + FMath::FRandRange(-150.0f, 150.0f);
            PosY = PosJugador.Y + FMath::FRandRange(-150.0f, 150.0f);
        }
        else
        {
            // Si no hay jugador, caer en el centro
            PosX = FMath::FRandRange(-300.0f, 300.0f);
            PosY = FMath::FRandRange(-300.0f, 300.0f);
        }

        // Cada plataforma aparece más arriba para que caigan en cadena
        float PosZ = 600.0f + (i * 150.0f);

        FVector Posicion(PosX, PosY, PosZ);

        // Tipo aleatorio: Indestructible (verde) o Destructible (rojo)
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
}