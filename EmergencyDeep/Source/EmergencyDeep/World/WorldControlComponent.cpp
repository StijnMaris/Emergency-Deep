#include "WorldControlComponent.h"
#include "WorldGenerator.h"

// Sets default values for this component's properties
UWorldControlComponent::UWorldControlComponent() : MaxSpeed(100.f), CurrentSpeed(0.f), Acceleration(10.f), Deceleration(20.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWorldControlComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UWorldControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsMoving)
	{
		if (CurrentSpeed < MaxSpeed)
		{
			CurrentSpeed += Acceleration * DeltaTime;
		}
		else
		{
			CurrentSpeed = MaxSpeed;
		}
	}
	else
	{
		if (CurrentSpeed > 0.f)
		{
			CurrentSpeed -= Deceleration * DeltaTime;
		}
		else
		{
			CurrentSpeed = 0.f;
		}
	}

	auto worldGenerator = AWorldGenerator::ActiveWorldGenerator;
	if (worldGenerator)
	{
		worldGenerator->MoveWorld(CurrentSpeed * DeltaTime);
	}
}

void UWorldControlComponent::StartMove()
{
	IsMoving = true;
}

void UWorldControlComponent::StopMove()
{
	IsMoving = false;
}

void UWorldControlComponent::Rotate(float angle)
{
	auto worldGenerator = AWorldGenerator::ActiveWorldGenerator;
	if (worldGenerator)
	{
		worldGenerator->RotateWorld(angle);
	}
}

