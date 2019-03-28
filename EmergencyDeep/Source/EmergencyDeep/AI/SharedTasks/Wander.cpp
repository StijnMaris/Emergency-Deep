#include "Wander.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "../SwimMovementComponent.h"
#include "EmergencyDeep.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

UWander::UWander(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer), TargetWaypoint(nullptr)
{
	bNotifyTick = true;
}

void UWander::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!TargetWaypoint) return;

	// Check distance to target
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<USwimMovementComponent*> SwimComponents;
	controlledPawn->GetComponents<USwimMovementComponent>(SwimComponents);

	if (SwimComponents.Num() <= 0)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("UWander::TickTask > Missing USwimMovementComponent on controlled pawn!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	//AAIController* controller = OwnerComp.GetAIOwner();
	//if (controller)
	//{
		//UBlackboardComponent* blackboardComp = controller->GetBlackboardComponent();

		auto vecToTarget = TargetWaypoint->GetActorLocation() - controlledPawn->GetActorLocation();
		auto dist = vecToTarget.Size();

		SwimComponents[0]->SetTargetLocation(TargetWaypoint->GetActorLocation());

		if (dist <= AcceptanceRadius)
		{
			GetWorld()->DestroyActor(TargetWaypoint);
			TargetWaypoint = nullptr;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	//}
}

EBTNodeResult::Type UWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto location = controlledPawn->GetActorLocation();
	int sign = FMath::RandRange(0, 1) * 2 - 1;
	auto randomVec = FVector(FMath::RandRange(sign * MinimumWanderArea.X, sign * MaximumWanderArea.X), FMath::RandRange(sign * MinimumWanderArea.Y, sign * MaximumWanderArea.Y), FMath::RandRange(sign * MinimumWanderArea.Z, sign * MaximumWanderArea.Z));

	auto wanderLocation = location + randomVec;

	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	TargetWaypoint = GetWorld()->SpawnActor<AAIWayPoint>(wanderLocation, Rotation, SpawnInfo);
	FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

	auto parent = controlledPawn->GetRootComponent()->GetAttachParent()->GetOwner();
	TargetWaypoint->AttachToActor(parent, attachRules);

	return EBTNodeResult::InProgress;
}

void UWander::OnGameplayTaskActivated(UGameplayTask &)
{
}
