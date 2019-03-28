#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetWanderLocation.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UGetWanderLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &) override;

	UPROPERTY(EditAnywhere)
	FVector MaximumWanderArea;

	UPROPERTY(EditAnywhere)
	FVector MinimumWanderArea;
};
