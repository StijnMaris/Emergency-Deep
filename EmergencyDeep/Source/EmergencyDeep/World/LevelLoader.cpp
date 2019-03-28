#include "LevelLoader.h"
#include "Engine/LevelStreaming.h"

ALevelLoader *ALevelLoader::LevelLoader = nullptr;

// Sets default values
ALevelLoader::ALevelLoader() : CurrentStreamingAction(FStreamingAction::Empty())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bNetLoadOnClient = false;
}

// Called when the game starts or when spawned
void ALevelLoader::BeginPlay()
{
	Super::BeginPlay();

	LevelLoader = this;
}

// Called every frame
void ALevelLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		switch (CurrentStreamingAction.StreamingType)
		{
		case EStreamingType::ST_Idle:
			if (QeuedLoadOperations.Num() > 0)
			{
				CurrentStreamingAction = QeuedLoadOperations[0];

				if (CurrentStreamingAction.LevelName != "")
				{
					switch (CurrentStreamingAction.StreamingType)
					{
					case EStreamingType::ST_Load:
						UGameplayStatics::LoadStreamLevel(LevelContext, CurrentStreamingAction.LevelName, true, true, CurrentLatentActionInfo);
						break;

					case EStreamingType::ST_Unload:
						UGameplayStatics::UnloadStreamLevel(LevelContext, CurrentStreamingAction.LevelName, CurrentLatentActionInfo, true);
						break;
					}
				}

				QeuedLoadOperations.Remove(CurrentStreamingAction);
			}
			break;

		case EStreamingType::ST_Load:
			if (UGameplayStatics::GetStreamingLevel(LevelContext, CurrentStreamingAction.LevelName)->IsLevelLoaded())
			{
				CurrentStreamingAction = FStreamingAction::Empty();
			}
			break;

		case EStreamingType::ST_Unload:
			CurrentStreamingAction = FStreamingAction::Empty();
			break;
		}
	}
}

void ALevelLoader::LoadLevel(const FName &level)
{
	if (!LevelContext && !HasAuthority()) return;
	QeuedLoadOperations.Add(FStreamingAction(level, EStreamingType::ST_Load));
}

void ALevelLoader::UnLoadLevel(const FName &level)
{
	if (!LevelContext && !HasAuthority()) return;
	QeuedLoadOperations.Add(FStreamingAction(level, EStreamingType::ST_Unload));
}
