#include "SwimMovementComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USwimMovementComponent::USwimMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSpeed = 0.f;
	DesiredSpeed = 0.f;
	Acceleration = 5.f;
	Deceleration = 5.f;
	RotationSpeed = 15.f;
	MaxSpeed = 250.f;
	Velocity = FVector(0.f, 0.f, 0.f);
	DesiredVelocity = FVector(0.f, 0.f, 0.f);
}


// Called when the game starts
void USwimMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetSwimLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void USwimMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDesiredVelocity();
	UpdateVelocity(DeltaTime);
	UpdateLocation(DeltaTime);
}

void USwimMovementComponent::SetTargetLocation(const FVector &Target)
{
	TargetSwimLocation = Target;
}

const FVector &USwimMovementComponent::GetVelocity() const
{
	return Velocity;
}

void USwimMovementComponent::UpdateDesiredVelocity()
{
	DesiredVelocity = TargetSwimLocation - GetOwner()->GetActorLocation();

	if (DesiredVelocity.Size() >= MaxSpeed)
	{
		DesiredVelocity.Normalize();
		DesiredVelocity *= MaxSpeed;
	}
}

void USwimMovementComponent::UpdateVelocity(float DeltaTime)
{
	if (CurrentSpeed < DesiredSpeed)
	{
		CurrentSpeed += Acceleration * DeltaTime;
	}
	else
	{
		CurrentSpeed -= Deceleration * DeltaTime;
	}

	Velocity = GetOwner()->GetActorForwardVector() * CurrentSpeed;

	auto vecToDesiredVelo = (DesiredVelocity - Velocity);

	DesiredSpeed = (vecToDesiredVelo + Velocity).Size();

	vecToDesiredVelo.Normalize();
	auto upVec = FVector::CrossProduct(Velocity, -vecToDesiredVelo);
	auto changeVec = FVector::CrossProduct(Velocity, upVec);
	changeVec.Normalize();

	auto newForward = GetOwner()->GetActorForwardVector() + changeVec * RotationSpeed * DeltaTime;
	newForward.Normalize();

	GetOwner()->SetActorRotation(newForward.Rotation());
}

void USwimMovementComponent::UpdateLocation(float DeltaTime)
{
	auto location = GetOwner()->GetActorLocation();
	location += Velocity * DeltaTime;
	GetOwner()->SetActorLocation(location, true);
}
