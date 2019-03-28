#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KD_ChargeShip.generated.h"

class AHullBreach;
class UHullBreachHolderComponent;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UKD_ChargeShip : public UBTTaskNode
{
	GENERATED_BODY()
	
	UKD_ChargeShip(const FObjectInitializer& objectInitializer);

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &) override;
	virtual void TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;

private:
	void FindClosestBreach(UBehaviorTreeComponent& OwnerComp, UHullBreachHolderComponent *BreachesHolder);

	AHullBreach *TargetBreach;
};
