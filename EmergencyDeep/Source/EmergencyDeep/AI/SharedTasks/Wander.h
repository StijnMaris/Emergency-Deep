#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../AIWayPoint.h"
#include "Wander.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UWander : public UBTTaskNode
{
	GENERATED_BODY()

	UWander(const FObjectInitializer& objectInitializer);
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &) override;
	virtual void TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere)
	FVector MaximumWanderArea;

	UPROPERTY(EditAnywhere)
	FVector MinimumWanderArea;

private:
	AAIWayPoint *TargetWaypoint;
};
