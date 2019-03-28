#include "LevelBoundary.h"
#include "WorldGenerator.h"

// Sets default values
ALevelBoundary::ALevelBoundary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bGenerateOverlapEventsDuringLevelStreaming = true;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	BoundaryType = ELevelBoundaryType::BT_LoadNew;

	OverlapVolume->OnComponentBeginOverlap.__Internal_AddUniqueDynamic(this, &ALevelBoundary::OverlapBegins, TEXT("OverlapBegins"));
}

// Called when the game starts or when spawned
void ALevelBoundary::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelBoundary::OverlapBegins(UPrimitiveComponent *OverlapComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!bDidGeneration)
	{
		ACharacter *Character = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (OtherActor == Character)
		{
			switch (BoundaryType)
			{
			case ELevelBoundaryType::BT_LoadNew:
				AWorldGenerator::ActiveWorldGenerator->LoadNext();
				break;
			case ELevelBoundaryType::BT_Unload:
				AWorldGenerator::ActiveWorldGenerator->UnloadLast();
				break;
			}
			bDidGeneration = true;
		}
	}
}

// Called every frame
void ALevelBoundary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

