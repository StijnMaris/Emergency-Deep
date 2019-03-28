#include "CaveSegmentController.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.h"

// Sets default values for this component's properties
UCaveSegmentController::UCaveSegmentController() : WasAdded(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCaveSegmentController::BeginPlay()
{
	Super::BeginPlay();

	if(AWorldGenerator::ActiveWorldGenerator)
		AWorldGenerator::ActiveWorldGenerator->RegisterNewCaveSegment(this);

	WasAdded = true;
}

// Called every frame
void UCaveSegmentController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCaveSegmentController::BeginDestroy()
{
	Super::BeginDestroy();

	if (AWorldGenerator::ActiveWorldGenerator)
		if(WasAdded)
			AWorldGenerator::ActiveWorldGenerator->OnDestroyCaveSegment(this);
}

void UCaveSegmentController::Move(float Speed)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	CurrentLocation.Z += Speed;
	GetOwner()->SetActorLocation(CurrentLocation);

	for (auto foliage : AttachedFoliage)
		if (foliage)
			foliage->Move(Speed);
}

void UCaveSegmentController::UpdateRotation(float Rotation)
{
	FVector RotationVec(0.f, 0.f, Rotation);
	GetOwner()->SetActorRotation(FQuat::MakeFromEuler(RotationVec));

	for (auto foliage : AttachedFoliage)
		if (foliage)
			foliage->UpdateRotation(Rotation);
}

void UCaveSegmentController::AddFoliage(UCaveFoliage *foliage)
{
	if (foliage)
		AttachedFoliage.Add(foliage);
}