#include "KD_ChargeShip.h"
#include "Tasks/Objects/HullBreach.h"
#include "AI/HullBreachHolderComponent.h"
#include "Submarine/SubmarineAIDataActor.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

UKD_ChargeShip::UKD_ChargeShip(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	AcceptanceRadius = 100.0f;
	bNotifyTick = true;
}

EBTNodeResult::Type UKD_ChargeShip::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto breaches = ASubmarineAIDataActor::SubmarineAIDataActor->HullBreaches;
	if(!breaches) return EBTNodeResult::Failed;

	FindClosestBreach(OwnerComp, breaches);

	if (!TargetBreach)
	{
		// All breaches already open
		return EBTNodeResult::Succeeded;
	}

	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto pParentComp = controlledPawn->GetRootComponent()->GetAttachParent();

	if (pParentComp)
	{
		auto pParent = pParentComp->GetOwner();
		FDetachmentTransformRules detachRules(EDetachmentRule::KeepWorld, true);
		if (pParent)
		{
			AAIController* controller = OwnerComp.GetAIOwner();
			if (controller)
			{
				UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();
				blackboardComp->SetValueAsObject(TEXT("ParentRoom"), pParent);
			}
			controlledPawn->DetachFromActor(detachRules);
		}
	}

	return EBTNodeResult::InProgress;
}

void UKD_ChargeShip::OnGameplayTaskActivated(UGameplayTask &)
{
}

void UKD_ChargeShip::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	// Check distance to target
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<USwimMovementComponent*> SwimComponents;
	controlledPawn->GetComponents<USwimMovementComponent>(SwimComponents);

	if (SwimComponents.Num() <= 0)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("USwimTowardsWhileLookingAt::ExecuteTask > Missing USwimMovementComponent on controlled pawn!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	auto vecToTarget = TargetBreach->GetActorLocation() - controlledPawn->GetActorLocation();
	auto dist = vecToTarget.Size();

	SwimComponents[0]->SetTargetLocation(TargetBreach->GetActorLocation());

	if (dist <= AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UKD_ChargeShip::FindClosestBreach(UBehaviorTreeComponent &OwnerComp, UHullBreachHolderComponent *BreachesHolder)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto location = controlledPawn->GetActorLocation();

	float closestDistance = 10000000;
	size_t closestID = -1;
	for (size_t i = 0; i < BreachesHolder->HullBreaches.Num(); ++i)
	{
		auto breach = BreachesHolder->HullBreaches[i];
		if (breach->Broken && !breach->Corked) continue;

		auto breachLocation = breach->GetActorLocation();
		auto vetToTarget = breachLocation - location;
		auto dist = vetToTarget.Size();

		if (dist < closestDistance)
		{
			closestDistance = dist;
			closestID = i;
		}
	}

	if (closestID != -1)
		TargetBreach = BreachesHolder->HullBreaches[closestID];
}
