#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubmarineAIDataActor.generated.h"

class UWaypointHolderComponent;
class UHullBreachHolderComponent;

UCLASS()
class EMERGENCYDEEP_API ASubmarineAIDataActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubmarineAIDataActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UWaypointHolderComponent *WaypointsAroundShip;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UHullBreachHolderComponent *HullBreaches;

	static ASubmarineAIDataActor *SubmarineAIDataActor;
};
