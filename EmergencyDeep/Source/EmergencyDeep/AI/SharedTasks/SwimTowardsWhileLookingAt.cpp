#include "SwimTowardsWhileLookingAt.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

USwimTowardsWhileLookingAt::USwimTowardsWhileLookingAt(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	bNotifyTick = true;
}

void USwimTowardsWhileLookingAt::OnGameplayTaskActivated(UGameplayTask &)
{
}

void USwimTowardsWhileLookingAt::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance to target
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();

		auto target = blackboardComp->GetValueAsVector("Target");
		auto vecToTarget = target - controlledPawn->GetActorLocation();
		auto dist = vecToTarget.Size();

		if (dist <= AcceptanceRadius)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type USwimTowardsWhileLookingAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	TArray<USwimMovementComponent*> SwimComponents;
	controlledPawn->GetComponents<USwimMovementComponent>(SwimComponents);

	if (SwimComponents.Num() <= 0)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("USwimTowardsWhileLookingAt::ExecuteTask > Missing USwimMovementComponent on controlled pawn!"));
		return EBTNodeResult::Failed;
	}

	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();
		//APawn* plyPawn = Cast<APawn>(blackboardComp->GetValueAsObject("Target"));

		auto target = blackboardComp->GetValueAsVector("Target");

		SwimComponents[0]->SetTargetLocation(target);
	}

	return EBTNodeResult::InProgress;
}
