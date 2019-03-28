#include "SonarDetectableObjectComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
USonarDetectableObjectComponent::USonarDetectableObjectComponent() : SonarPingColor(FColor(255, 255, 255, 255))
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bOverrideLocation = false;
}

// Called when the game starts
void USonarDetectableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USonarDetectableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector USonarDetectableObjectComponent::GetSonarLocation_Implementation()
{
	if(bOverrideLocation)
		return OverrideLocation;

	return GetOwner()->GetActorLocation();
}

