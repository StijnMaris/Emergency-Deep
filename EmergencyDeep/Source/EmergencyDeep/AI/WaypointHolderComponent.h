#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIWayPoint.h"
#include "WaypointHolderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UWaypointHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaypointHolderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetNextWayPoint();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Waypoints;
	
private:
	int CurrentWaypoint;
};
