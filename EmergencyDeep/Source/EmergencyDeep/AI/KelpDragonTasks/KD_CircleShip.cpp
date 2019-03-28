#include "KD_CircleShip.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"
#include "../WaypointHolderComponent.h"
#include "Submarine/SubmarineAIDataActor.h"

UKD_CircleShip::UKD_CircleShip(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	bNotifyTick = true;

	MinCirclingTime = 60.0f;
	MaxCirclingTime = 120.0f;
}

void UKD_CircleShip::OnGameplayTaskActivated(UGameplayTask &)
{
}

void UKD_CircleShip::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance to target
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<USwimMovementComponent*> SwimComponents;
	controlledPawn->GetComponents<USwimMovementComponent>(SwimComponents);

	if (SwimComponents.Num() <= 0)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("UKD_CircleShip::TickTask > Missing USwimMovementComponent on controlled pawn!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (!WayPoints)
	{
		ED_LOG(LogED_Game, Warning, TEXT(""), TEXT("UKD_CircleShip::TickTask > Missing waypoints!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (WayPoints->Waypoints.Num() <= 0)
	{
		ED_LOG(LogED_Game, Warning, TEXT(""), TEXT("UKD_CircleShip::TickTask > Missing waypoints!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	auto targetWaypoint = WayPoints->Waypoints[CurrentWaypoint];

	if (!targetWaypoint)
	{
		ED_LOG(LogED_Game, Warning, TEXT(""), TEXT("UKD_CircleShip::TickTask > Missing waypoints!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	auto vecToTarget = targetWaypoint->GetActorLocation() - controlledPawn->GetActorLocation();
	auto dist = vecToTarget.Size();

	SwimComponents[0]->SetTargetLocation(targetWaypoint->GetActorLocation());

	if (dist <= AcceptanceRadius)
	{
		++CurrentWaypoint;
		if (CurrentWaypoint >= WayPoints->Waypoints.Num())
		{
			CurrentWaypoint = 0;
		}
	}

	CirclingTimer -= DeltaSeconds;
	if (CirclingTimer <= 0.0f)
	{
		CirclingTimer = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

EBTNodeResult::Type UKD_CircleShip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!ASubmarineAIDataActor::SubmarineAIDataActor) return EBTNodeResult::Failed;
	WayPoints = ASubmarineAIDataActor::SubmarineAIDataActor->WaypointsAroundShip;

	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{
		UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();
		auto pParent = blackboardComp->GetValueAsObject(TEXT("ParentRoom"));
		auto pParentActor = Cast<AActor>(pParent);
		if (pParentActor)
		{
			// Attach
			FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
			auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
			controlledPawn->AttachToActor(pParentActor, attachRules);
		}
	}

	FindClosestWayPoint(OwnerComp);

	CirclingTimer = FMath::RandRange(MinCirclingTime, MaxCirclingTime);

	return EBTNodeResult::InProgress;
}

void UKD_CircleShip::FindClosestWayPoint(UBehaviorTreeComponent& OwnerComp)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto location = controlledPawn->GetActorLocation();

	float closestDistance = 10000000;
	size_t closestID = -1;
	for (size_t i = 0; i < WayPoints->Waypoints.Num(); ++i)
	{
		auto wayPoint = WayPoints->Waypoints[i];
		auto wayPointLocation = wayPoint->GetActorLocation();
		auto vetToTarget = wayPointLocation - location;
		auto dist = vetToTarget.Size();

		if (dist < closestDistance)
		{
			closestDistance = dist;
			closestID = i;
		}
	}

	if (closestID != -1)
		CurrentWaypoint = closestID;
}
