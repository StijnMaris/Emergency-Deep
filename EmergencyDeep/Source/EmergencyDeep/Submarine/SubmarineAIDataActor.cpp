#include "SubmarineAIDataActor.h"
#include "../AI/WaypointHolderComponent.h"
#include "../AI/HullBreachHolderComponent.h"

ASubmarineAIDataActor *ASubmarineAIDataActor::SubmarineAIDataActor = nullptr;

// Sets default values
ASubmarineAIDataActor::ASubmarineAIDataActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	WaypointsAroundShip = CreateDefaultSubobject<UWaypointHolderComponent>(TEXT("WaypointsAroundShip"));
	HullBreaches = CreateDefaultSubobject<UHullBreachHolderComponent>(TEXT("HullBreaches"));

	SubmarineAIDataActor = this;
}

// Called when the game starts or when spawned
void ASubmarineAIDataActor::BeginPlay()
{
	Super::BeginPlay();

	SubmarineAIDataActor = this;
}

// Called every frame
void ASubmarineAIDataActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

