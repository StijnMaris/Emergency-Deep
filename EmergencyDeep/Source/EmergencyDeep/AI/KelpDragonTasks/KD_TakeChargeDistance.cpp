#include "KD_TakeChargeDistance.h"
#include "../AIWayPoint.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

UKD_TakeChargeDistance::UKD_TakeChargeDistance(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	bNotifyTick = true;

	AcceptanceRadius = 150.0f;
	MinDistance = 500.0f;
}

EBTNodeResult::Type UKD_TakeChargeDistance::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();

		FVector direction = controlledPawn->GetActorLocation();
		direction.Z = 0.0f;
		direction.Normalize();
		auto target = direction * MinDistance;

		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		TargetWaypoint = GetWorld()->SpawnActor<AAIWayPoint>(target, Rotation, SpawnInfo);
	}

	return EBTNodeResult::InProgress;
}

void UKD_TakeChargeDistance::OnGameplayTaskActivated(UGameplayTask &)
{
}

void UKD_TakeChargeDistance::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!TargetWaypoint) return;

	// Check distance to target
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<USwimMovementComponent*> SwimComponents;
	controlledPawn->GetComponents<USwimMovementComponent>(SwimComponents);

	if (SwimComponents.Num() <= 0)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("UKD_TakeChargeDistance::TickTask > Missing USwimMovementComponent on controlled pawn!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	auto vecToTarget = TargetWaypoint->GetActorLocation() - controlledPawn->GetActorLocation();
	auto dist = vecToTarget.Size();

	SwimComponents[0]->SetTargetLocation(TargetWaypoint->GetActorLocation());

	if (dist <= AcceptanceRadius)
	{
		GetWorld()->DestroyActor(TargetWaypoint);
		TargetWaypoint = nullptr;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
