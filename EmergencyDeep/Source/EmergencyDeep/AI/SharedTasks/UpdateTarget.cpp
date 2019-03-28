#include "UpdateTarget.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

void UUpdateTarget::OnGameplayTaskActivated(UGameplayTask &)
{
}

EBTNodeResult::Type UUpdateTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	TArray<USwimMovementComponent*> SwimComponents;
	controlledPawn->GetComponents<USwimMovementComponent>(SwimComponents);

	if (SwimComponents.Num() <= 0)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("UUpdateTarget::ExecuteTask > Missing USwimMovementComponent on controlled pawn!"));
		return EBTNodeResult::Failed;
	}

	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();

		auto target = blackboardComp->GetValueAsVector("Target");

		SwimComponents[0]->SetTargetLocation(target);
	}

	return EBTNodeResult::Succeeded;
}