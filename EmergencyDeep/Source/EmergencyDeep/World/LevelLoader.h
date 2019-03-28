#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "LevelLoader.generated.h"

UENUM()
enum class EStreamingType : uint8
{
	ST_Idle,
	ST_Load,
	ST_Unload,
};

USTRUCT()
struct FStreamingAction
{
	GENERATED_BODY()

	FStreamingAction() : LevelName(""), StreamingType(EStreamingType::ST_Idle) {}
	FStreamingAction(const FName &levelName, const EStreamingType &streamingType) : LevelName(levelName), StreamingType(streamingType) {}

	FName LevelName;
	EStreamingType StreamingType;

	static FStreamingAction Empty()
	{
		return FStreamingAction(TEXT(""), EStreamingType::ST_Idle);
	}

	bool operator==(const FStreamingAction &otherOperation)
	{
		return(LevelName == otherOperation.LevelName && StreamingType == otherOperation.StreamingType);
	}
};

UCLASS()
class EMERGENCYDEEP_API ALevelLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelLoader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LoadLevel(const FName &level);

	UFUNCTION(BlueprintCallable)
	void UnLoadLevel(const FName &level);

public:
	UPROPERTY(EditAnywhere)
	AActor *LevelContext;

	static ALevelLoader *LevelLoader;

private:
	UPROPERTY()
	TArray<FStreamingAction> QeuedLoadOperations;

	UPROPERTY()
	FStreamingAction CurrentStreamingAction;

	UPROPERTY()
	FLatentActionInfo CurrentLatentActionInfo;
};
