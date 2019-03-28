#include "HarpoonDispenser.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/World.h"
#include "Props/BaseProjectile.h"

AHarpoonDispenser::AHarpoonDispenser() : SpawnCounter(0)
{
	DispenserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DispenserMesh"));
	RootComponent = DispenserMesh;

	DispensePositioner = CreateDefaultSubobject<USceneComponent>(TEXT("DispensePositioner"));
	DispensePositioner->SetupAttachment(DispenserMesh);

	SpawnLimit = 100;
}

void AHarpoonDispenser::BeginPlay()
{
	Super::BeginPlay();
}

void AHarpoonDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHarpoonDispenser::Dispense()
{
	if (SpawnCounter >= SpawnLimit)
	{
		OnDispenseMaxed_BP();
		return;
	}

	if (ToSpawn)
	{
		UWorld *world = GetWorld();
		if (world)
		{
			FVector location = DispensePositioner->GetComponentLocation();
			FRotator rotation = DispensePositioner->GetComponentRotation();
			FActorSpawnParameters spawnParams{};
			spawnParams.Owner = this;
			auto projectile = world->SpawnActor<ABaseProjectile>(ToSpawn, location, rotation, spawnParams);
			if (projectile)
			{
				projectile->Dispenser = this;
				++SpawnCounter;

				OnDispense_BP();
			}
		}
	}
}

void AHarpoonDispenser::ReduceSpawnCount()
{
	--SpawnCounter;
}