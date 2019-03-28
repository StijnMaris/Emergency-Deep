#include "WorldGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "LevelLoader.h"
#include "Game/GameStates/EDGameGameState.h"

AWorldGenerator *AWorldGenerator::ActiveWorldGenerator = nullptr;

AWorldGenerator::AWorldGenerator() : CurrentDepth(0.f), CurrentWorldRotation(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = 1.0f;
	RotationTresshold = 1.0f;
}

void AWorldGenerator::BeginPlay()
{
	ActiveWorldGenerator = this;

	Super::BeginPlay();

	if (StartLevelToLoad != "")
	{
		ALevelLoader::LevelLoader->LoadLevel(StartLevelToLoad);
		CurrentLoadedLevelName = StartLevelToLoad;
	}

	DesiredWorldRotation = CurrentWorldRotation;

	TotalRotateDelta = 0.0f;
}

void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TotalRotateDelta > RotationTresshold)
	{
		TotalRotateDelta -= DeltaTime * RotationSpeed;
		DesiredWorldRotation += DeltaTime * RotationSpeed;
	}
	else if (TotalRotateDelta < -RotationTresshold)
	{
		TotalRotateDelta += DeltaTime * RotationSpeed;
		DesiredWorldRotation -= DeltaTime * RotationSpeed;
	}

	float lastRotation = CurrentWorldRotation;
	CurrentWorldRotation = FMath::Lerp<float>(CurrentWorldRotation, DesiredWorldRotation, 0.01f);
	if (lastRotation != CurrentWorldRotation)
		UpdateWorldRotations();

	AEDGameGameState* gameState = Cast<AEDGameGameState>(UGameplayStatics::GetGameState(GetWorld()));
	gameState->SetScore(CurrentDepth);
}

void AWorldGenerator::MoveWorld(float Speed)
{
	for (auto Segment : ActiveCaveSegments)
	{
		if (IsValid(Segment))
			Segment->Move(Speed);
		else
			ActiveCaveSegments.Remove(Segment);
	}
	CurrentDepth += Speed;
}

void AWorldGenerator::LoadNext()
{
	FName LevelToLoad = SelectSegmentToSpawn();

	ALevelLoader::LevelLoader->LoadLevel(LevelToLoad);
	PreviouslyLoadedLevelName = CurrentLoadedLevelName;
	CurrentLoadedLevelName = LevelToLoad;

	UpdateWorldRotations();
}

void AWorldGenerator::RegisterNewCaveSegment(UCaveSegmentController *Segment)
{
	if (IsValid(Segment))
		ActiveCaveSegments.Add(Segment);
}

void AWorldGenerator::OnDestroyCaveSegment(UCaveSegmentController *Segment)
{
	ActiveCaveSegments.Remove(Segment);
}

void AWorldGenerator::UnloadLast()
{
	ALevelLoader::LevelLoader->UnLoadLevel(PreviouslyLoadedLevelName);
}

const FName AWorldGenerator::SelectSegmentToSpawn() const
{
	float LowDepthChance = 100.f;
	float MediumDepthChance = 0.f;
	float HighDepthChance = 0.f;

	// Chance calculation
	if (CurrentDepth < LowDepth)
	{
		LowDepthChance = 100.f;
	}
	else if (CurrentDepth < MediumDepth)
	{
		MediumDepthChance = MediumDepthEnvironmentChance;
		HighDepthChance = HighDepthEnvironmentChance;
	}
	else if (CurrentDepth < HighDepth)
	{
		MediumDepthChance = MediumDepthEnvironmentChance * 2.f;
		HighDepthChance = HighDepthEnvironmentChance;
	}
	else
	{
		MediumDepthChance = MediumDepthEnvironmentChance;
		HighDepthChance = HighDepthEnvironmentChance * 2.f;
	}

	LowDepthChance = 100.f - (MediumDepthChance + HighDepthChance);
	if (LowDepthChance < 0.f) LowDepthChance = 0.f;

	// Pick which type to spawn
	float Random = FMath::RandRange(0.f, 100.f);
	float Low = LowDepthChance;
	float Medium = Low + MediumDepthChance;
	float High = Medium + HighDepthChance;
	if (Random < Low)
		return PickLowDepthEnvironment();
	else if (Random < Medium)
		return PickMediumDepthEnvironment();
	else
		return PickMediumDepthEnvironment();
}

const FName AWorldGenerator::PickLowDepthEnvironment() const
{
	TArray<FName> PossibleLevels;
	for (auto Level : LowDepthEnvironmentLevels)
		if (Level != CurrentLoadedLevelName)
			PossibleLevels.Add(Level);

	int Count = PossibleLevels.Num();
	int RandomIndex = FMath::RandRange(0, Count - 1);
	return PossibleLevels[RandomIndex];
}

const FName AWorldGenerator::PickMediumDepthEnvironment() const
{
	TArray<FName> PossibleLevels;
	for (auto Level : MediumDepthEnvironmentLevels)
		if (Level != CurrentLoadedLevelName)
			PossibleLevels.Add(Level);

	int Count = PossibleLevels.Num();
	int RandomIndex = FMath::RandRange(0, Count - 1);
	return PossibleLevels[RandomIndex];
}

const FName AWorldGenerator::PickHighDepthEnvironment() const
{
	TArray<FName> PossibleLevels;
	for (auto Level : HighDepthEnvironmentLevels)
		if (Level != CurrentLoadedLevelName)
			PossibleLevels.Add(Level);

	int Count = PossibleLevels.Num();
	int RandomIndex = FMath::RandRange(0, Count - 1);
	return PossibleLevels[RandomIndex];
}

void AWorldGenerator::RotateWorld(float Rotation)
{
	//DesiredWorldRotation = Rotation;
	TotalRotateDelta += Rotation * DeltaRotationScale;
	UpdateWorldRotations();
}

float AWorldGenerator::GetTotalDelta()
{
	if (TotalRotateDelta <= RotationTresshold && TotalRotateDelta >= -RotationTresshold)
	{
		return 0.0f;
	}
	return TotalRotateDelta;
}

float AWorldGenerator::GetDesiredRotation()
{
	return DesiredWorldRotation;
}

UCaveSegmentController *AWorldGenerator::GetLastSegmentController()
{
	if(ActiveCaveSegments.Num() <= 0)
		return nullptr;
	
	return ActiveCaveSegments[ActiveCaveSegments.Num() - 1];
}

void AWorldGenerator::UpdateWorldRotations()
{
	for (auto Segment : ActiveCaveSegments)
		if (IsValid(Segment))
			Segment->UpdateRotation(CurrentWorldRotation);
}