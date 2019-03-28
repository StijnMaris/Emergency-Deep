#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SwimTowardsWhileLookingAt.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API USwimTowardsWhileLookingAt : public UBTTaskNode
{
	GENERATED_BODY()

	USwimTowardsWhileLookingAt(const FObjectInitializer& objectInitializer);
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;
};
