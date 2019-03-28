#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KD_TakeChargeDistance.generated.h"

class AAIWayPoint;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UKD_TakeChargeDistance : public UBTTaskNode
{
	GENERATED_BODY()

	UKD_TakeChargeDistance(const FObjectInitializer& objectInitializer);

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &) override;
	virtual void TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere)
	float MinDistance;

private:
	AAIWayPoint *TargetWaypoint;
};
