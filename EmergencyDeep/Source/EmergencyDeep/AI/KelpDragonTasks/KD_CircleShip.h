

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KD_CircleShip.generated.h"

class UWaypointHolderComponent;
class AAIWayPoint;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UKD_CircleShip : public UBTTaskNode
{
	GENERATED_BODY()

	UKD_CircleShip(const FObjectInitializer& objectInitializer);

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &) override;
	virtual void TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere)
	UWaypointHolderComponent *WayPoints;

	UPROPERTY(VisibleAnywhere)
	uint8 CurrentWaypoint;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere)
	float MinCirclingTime;

	UPROPERTY(EditAnywhere)
	float MaxCirclingTime;

private:
	void FindClosestWayPoint(UBehaviorTreeComponent& OwnerComp);

private:
	float CirclingTimer;
};
