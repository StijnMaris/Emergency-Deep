#include "WaterLevel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/PostProcessComponent.h"
#include "Tasks/Objects/HullBreach.h"
#include "Tasks/Pump.h"
#include "Game/GameStates/EDGameGameState.h"

// Sets default values
AWaterLevel::AWaterLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	WaterPlaneUp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterPlaneUp"));
	WaterPlaneUnder = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("WaterPlaneUnder"));
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));

	RootComponent = Root;
	WaterPlaneUp->SetupAttachment(Root);
	WaterPlaneUnder->SetupAttachment(Root);
	Collider->SetupAttachment(Root);
	PostProcessComponent->SetupAttachment(Collider);
	PostProcessComponent->bUnbound = false;

	WaterPlaneUp->SetCollisionProfileName("OverlapAll");
	WaterPlaneUnder->SetCollisionProfileName("OverlapAll");
	Collider->SetCollisionProfileName("OverlapAll");

	Level = 0;
}

// Called when the game starts or when spawned
void AWaterLevel::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();

	AEDGameGameState* gameState = Cast<AEDGameGameState>(UGameplayStatics::GetGameState(GetWorld()));
	gameState->SetWaterLevelGameOverValue(WaterLevelMax);
}

void AWaterLevel::UpdateWater(float deltaTime)
{
	float WaterChange = 0;
	//Water Raise
	int Breaches = 0;
	for (int i = 0; i < HullBreaches.Num(); i++)
	{
		if (HullBreaches[i]->IsLeaking())
		{
			Breaches++;
		}
	}
	WaterChange += Breaches * WaterIncreaseRate;

	//Water Decrease

	int activePumps = 0;
	for (int i = 0; i < Pumps.Num(); i++)
	{
		if (Pumps[i]->IsPumping)
		{
			activePumps++;
		}
	}
	WaterChange -= activePumps * WaterDecreaseRate;
	//UE_LOG(LogTemp, Warning, TEXT("WaterChagne: %f"), WaterChange);

	//Water Change

	////CheckChange
	if (WaterChange == 0) return;

	Level += WaterChange * deltaTime;
	if (Level > WaterLevelMax)
	{
		Level = WaterLevelMax;
	}
	else if (Level < 0)
	{
		Level = 0;
	}
	
	UpdatePlane();
}

void AWaterLevel::UpdatePlane()
{
	FVector offset = GetActorUpVector()*Level;
	SetActorLocation(StartPosition + offset);
}

// Called every frame
void AWaterLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWater(DeltaTime);

	AEDGameGameState* gameState = Cast<AEDGameGameState>(UGameplayStatics::GetGameState(GetWorld()));
	gameState->SetWaterLevel(Level);
}

float AWaterLevel::GetWaterPercentage()
{
	return Level / WaterLevelMax;
}

