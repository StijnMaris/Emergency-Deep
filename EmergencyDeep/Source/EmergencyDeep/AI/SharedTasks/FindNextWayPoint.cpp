#include "FindNextWayPoint.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "../AIWayPoint.h"
#include "../WaypointHolderComponent.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"
#include "EmergencyDeep.h"

void UFindNextWayPoint::OnGameplayTaskActivated(UGameplayTask &)
{
}

EBTNodeResult::Type UFindNextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();
		//APawn* plyPawn = Cast<APawn>(blackboardComp->GetValueAsObject("Target"));

		auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
		TArray< UWaypointHolderComponent*> WaypointHolders;
		controlledPawn->GetComponents<UWaypointHolderComponent>(WaypointHolders);

		if (WaypointHolders.Num() <= 0)
		{
			ED_LOG(LogED_Game, Error, TEXT(""), TEXT("USwimTowardsWhileLookingAt::ExecuteTask > Missing USwimMovementComponent on controlled pawn!"));
			return EBTNodeResult::Failed;
		}

		FVector target = WaypointHolders[0]->GetNextWayPoint();

		blackboardComp->SetValueAsVector("Target", target);
	}

	return EBTNodeResult::Succeeded;
}