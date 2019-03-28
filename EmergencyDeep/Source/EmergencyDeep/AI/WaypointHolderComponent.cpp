#include "WaypointHolderComponent.h"

// Sets default values for this component's properties
UWaypointHolderComponent::UWaypointHolderComponent() : CurrentWaypoint(-1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UWaypointHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UWaypointHolderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UWaypointHolderComponent::GetNextWayPoint()
{
	if (Waypoints.Num() <= 0) return FVector(0.f, 0.f, 0.f);

	++CurrentWaypoint;
	if (CurrentWaypoint >= Waypoints.Num())
		CurrentWaypoint = 0;

	auto Waypoint = Waypoints[CurrentWaypoint];

	FVector Location = Waypoint->GetActorLocation();

	return Location;
}

