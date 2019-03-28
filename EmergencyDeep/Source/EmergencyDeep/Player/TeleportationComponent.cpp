
#include "TeleportationComponent.h"

#include "EDPlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UTeleportationComponent::UTeleportationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTeleportationComponent::BeginPlay()
{
	Super::BeginPlay();

	IgnoreActors.Add(GetOwner());
	IgnoreActors.Add(GetOwner()->GetOwner());
	// ...
	
}


// Called every frame
void UTeleportationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UTeleportationComponent::SetupComponent(UArrowComponent * arcDirection, USplineComponent * spline, UStaticMeshComponent * destinationMarker, AEDPlayerCharacter* character)
{
	ArcDirection = arcDirection;
	ArcSpline = spline;
	TeleportLocationMarker = destinationMarker;
	Character = character;

	if (arcDirection != nullptr &&
		spline != nullptr &&
		destinationMarker != nullptr &&
		character != nullptr)
	{
		bIsInit = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Setup TeleportationComponent not complete"));
	}
}

void UTeleportationComponent::UpdateDestinationMarker()
{
	if (bIsInit == false || bIsTeleporting) return;

	FVector start = ArcDirection->GetComponentLocation();
	FVector look = ArcDirection->GetForwardVector();
	TArray<FVector> path;

	// Trace path
	FHitResult hit;
	FPredictProjectilePathParams params(10.0f, start, look * TeleportProjectileSpeed, 30, ECollisionChannel::ECC_WorldStatic, GetOwner());
	params.ActorsToIgnore = IgnoreActors;
	FPredictProjectilePathResult result;
	bool bHit = UGameplayStatics::PredictProjectilePath(GetOwner(), params, result);

	// Get Path
	for (FPredictProjectilePathPointData pointData : result.PathData)
	{
		path.Add(pointData.Location);
	}
	/*if (bHit && result.HitResult.Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TeleportHitActor: %s"), *result.HitResult.Actor.Get()->GetName());
		if(result.HitResult.Component != nullptr)
			UE_LOG(LogTemp, Warning, TEXT("TeleportHitComp: %s"), *result.HitResult.Component->GetName());
		FVector offsetLog = result.HitResult.Location - start;
		UE_LOG(LogTemp, Warning, TEXT("TeleportHitPosFromPlayer: %s"), *offsetLog.ToString());
	}*/
	
	// Project on Navmesh
	FNavLocation navLocation;
	UNavigationSystemV1* navMesh = UNavigationSystemV1::GetCurrent(GetWorld());
	bool bOnNavMesh = false;
	if (navMesh != nullptr)
	{
		bOnNavMesh = navMesh->ProjectPointToNavigation(result.HitResult.Location, navLocation, TeleportProjection);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NoNavmesh"));
	}

	// Show Destination on Success
	if (bOnNavMesh && bHit)
	{
		TeleportLocationMarker->SetVisibility(true);
		TeleportLocationMarker->SetWorldLocation(navLocation.Location);
		bCanTeleport = true;
		UpdateSpline(path);
	}
	else
	{
		bCanTeleport = false;
		TeleportLocationMarker->SetVisibility(false);
		TeleportLocationMarker->SetWorldLocation(Character->GetActorLocation());
	}
}

void UTeleportationComponent::BeginTeleport()
{
	if (bIsInit == false) return;

	bIsTeleporting = true;

	// Set Visibility
	TeleportLocationMarker->SetVisibility(false);
	for (int32 i = 0; i < DynamicMeshes.Num(); i++)
	{
		DynamicMeshes[i]->SetVisibility(false);
	}

	// Fade and teleport
	TeleportationDestination = TeleportLocationMarker->GetComponentLocation();
	APlayerController* player = Cast<APlayerController>(Character->GetController());
	if (player != nullptr)
	{
		player->PlayerCameraManager->StartCameraFade(0, 1, TeleportFadeTime, FLinearColor::Black);
	}
	FTimerHandle handle;
	GetOwner()->GetWorldTimerManager().SetTimer(handle, this, &UTeleportationComponent::FinishTeleport, TeleportFadeTime);
}

bool UTeleportationComponent::CanTeleport() const
{
	return bCanTeleport;
}

bool UTeleportationComponent::IsTeleporting() const
{
	return bIsTeleporting;
}

void UTeleportationComponent::ResetDestination()
{
	if (bIsInit == false) return;
	TeleportLocationMarker->SetVisibility(false);
	for (int32 i = 0; i < DynamicMeshes.Num(); i++)
	{
		DynamicMeshes[i]->SetVisibility(false);
	}
	TeleportLocationMarker->SetWorldLocation(Character->GetActorLocation());
	TeleportationDestination = Character->GetActorLocation();
}

void UTeleportationComponent::FinishTeleport()
{
	// Teleport player to position
	APlayerController* player = Cast<APlayerController>(Character->GetController());
	if (player != nullptr)
	{
		player->PlayerCameraManager->StartCameraFade(1, 0, TeleportFadeTime, FLinearColor::Black);
	}
	FVector offset{0.0f};
	offset.Z = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Character->SetActorLocation(TeleportationDestination + offset);
	bIsTeleporting = false;
}

void UTeleportationComponent::UpdateSpline(const TArray<FVector>& path)
{
	if (bIsInit == false) return;

	ArcSpline->ClearSplinePoints(false);
	for (int32 i = 0; i < path.Num(); i++)
	{
		FVector localPosition = ArcSpline->GetComponentTransform().InverseTransformPosition(path[i]);
		FSplinePoint point(i, localPosition, ESplinePointType::Curve);
		ArcSpline->AddPoint(point, false);
	}


	ArcSpline->UpdateSpline();

	for (int32 i = 0; i < path.Num() - 1; i++)
	{
		if (DynamicMeshes.Num() <= i)
		{
			USplineMeshComponent* splineMesh = NewObject<USplineMeshComponent>(GetOwner());
			splineMesh->SetMobility(EComponentMobility::Movable);
			splineMesh->AttachToComponent(ArcSpline, FAttachmentTransformRules::KeepRelativeTransform);
			splineMesh->SetStaticMesh(ArcMesh);
			splineMesh->SetMaterial(0, ArcMaterial);
			splineMesh->RegisterComponent();
			DynamicMeshes.Add(splineMesh);
		}

		FVector startPos, startTang, endPos, endTang;
		ArcSpline->GetLocalLocationAndTangentAtSplinePoint(i, startPos, startTang);
		ArcSpline->GetLocalLocationAndTangentAtSplinePoint(i + 1, endPos, endTang);
		DynamicMeshes[i]->SetStartAndEnd(startPos, startTang, endPos, endTang);
		DynamicMeshes[i]->SetVisibility(true);
	}
	// Set remaining points off
	for (int32 i = path.Num(); i < DynamicMeshes.Num(); i++)
	{
		DynamicMeshes[i]->SetVisibility(false);
	}

}

