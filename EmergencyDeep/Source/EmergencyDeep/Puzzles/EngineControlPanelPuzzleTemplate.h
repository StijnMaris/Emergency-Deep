#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EngineControlPanel.h"
#include "EngineControlPanelPuzzleTemplate.generated.h"

struct FPuzzlePair;

UCLASS()
class EMERGENCYDEEP_API AEngineControlPanelPuzzleTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEngineControlPanelPuzzleTemplate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<FPuzzlePair> Puzzles;

	UPROPERTY(EditAnywhere)
	TArray<EButton> ButtonPairs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 PuzzleID;
};
