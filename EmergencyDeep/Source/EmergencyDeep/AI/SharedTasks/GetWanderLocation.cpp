#include "GetWanderLocation.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UGetWanderLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	//auto location = controlledPawn->GetActorLocation();
	int sign = FMath::RandRange(0, 1) * 2 - 1;
	auto randomVec = FVector(FMath::RandRange(sign * MinimumWanderArea.X, sign * MaximumWanderArea.X), FMath::RandRange(sign * MinimumWanderArea.Y, sign * MaximumWanderArea.Y), FMath::RandRange(sign * MinimumWanderArea.Z, sign * MaximumWanderArea.Z));

	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();

		blackboardComp->SetValueAsVector("WanderTarget", randomVec);
	}

	return EBTNodeResult::Succeeded;
}

void UGetWanderLocation::OnGameplayTaskActivated(UGameplayTask &)
{
}
