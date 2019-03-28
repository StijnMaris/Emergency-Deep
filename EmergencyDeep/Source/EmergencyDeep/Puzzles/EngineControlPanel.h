#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/PointLightComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "../Interactables/BaseButton.h"
#include "EngineControlPanel.generated.h"

class AEngineControlPanelPuzzleTemplate;
class UEDTask;
class ALightController;

UENUM(BlueprintType)
enum class EEngineFailureType : uint8
{
	EFT_Idle UMETA(DisplayName = "Idle"),
	EFT_Fire UMETA(DisplayName = "Fire"),
	EFT_Shutdown UMETA(DisplayName = "Shutdown"),
};

UENUM(BlueprintType)
enum class EEnginePuzzleType : uint8
{
	EFPT_Idle UMETA(DisplayName = "None"),
	EFPT_TableNumberButton UMETA(DisplayName = "Read Number"),
	EFPT_NumberButton UMETA(DisplayName = "Press Number"),
	EFPT_SliderPosition UMETA(DisplayName = "Set Slider Position"),
	EFPT_LeverToggle UMETA(DisplayName = "Pull Lever"),
};

UENUM(BlueprintType)
enum class EPuzzleColor : uint8
{
	PC_None UMETA(DisplayName = "None"),
	PC_Red UMETA(DisplayName = "Red"),
	PC_Green UMETA(DisplayName = "Green"),
	PC_Yellow UMETA(DisplayName = "Yellow"),
	PC_Blue UMETA(DisplayName = "Blue"),
};

UENUM(BlueprintType)
enum class EMistakeConcequence : uint8
{
	MC_None UMETA(DisplayName = "None"),
	MC_Reset UMETA(DisplayName = "Restart Puzzle"),
	MC_MistakeCounter UMETA(DisplayName = "Count Mistake"),
	MC_InstantFire UMETA(DisplayName = "Engine Fire"),
};

UENUM(BlueprintType)
enum class EButton : uint8
{
	B_Square UMETA(DisplayName = "Square"),
	B_Circle UMETA(DisplayName = "Circle"),
	B_Triangle UMETA(DisplayName = "Triangle"),
	B_Cross UMETA(DisplayName = "Cross"),
};

USTRUCT(BlueprintType)
struct FPuzzleAction
{
	GENERATED_BODY()

	FPuzzleAction() : PuzzleType(EEnginePuzzleType::EFPT_Idle), PuzzleData(0) {}

	FPuzzleAction(const EEnginePuzzleType &actionType, int puzzleData) :
		PuzzleType(actionType), PuzzleData(puzzleData) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Type")
	EEnginePuzzleType PuzzleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Data")
	int PuzzleData;
};

USTRUCT()
struct FPuzzlePair
{
	GENERATED_BODY()

	FPuzzlePair() : bValid(true), PuzzleAction(EEnginePuzzleType::EFPT_Idle, 0) {}

	FPuzzlePair(bool valid) : bValid(valid), PuzzleAction(EEnginePuzzleType::EFPT_Idle, 0) {}

	UPROPERTY(EditAnywhere)
	int PuzzleSequence;

	UPROPERTY(EditAnywhere)
	EPuzzleColor LightColor;

	UPROPERTY(EditAnywhere)
	FPuzzleAction PuzzleAction;

	UPROPERTY(EditAnywhere)
	EMistakeConcequence MistakeConcequence;

	bool bValid;

	static FPuzzlePair Invalid;
};

UCLASS()
class EMERGENCYDEEP_API AEngineControlPanel : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AEngineControlPanel(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFailure(const EEngineFailureType &FailureType);

	UFUNCTION(BlueprintCallable)
	void ForwardAction(const FPuzzleAction &action);

	UFUNCTION(BlueprintCallable)
	void ResetCounter();

	UFUNCTION(BlueprintCallable)
	void ResetEngine();

	UFUNCTION(BlueprintCallable)
	void SolveCurrentPuzzle();

	UFUNCTION(BlueprintCallable)
	void MadeMistake(const EMistakeConcequence &mistake);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMadeMistake"))
	void OnMadeMistake_BP(); // Making mistake in a sequence

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnCompleteFail"))
	void OnCompleteFail_BP(); // When made too many mistakes

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSequenceCorrect"))
	void OnSequenceCorrect_BP(); // When single sequence solved correctly

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEngineRestartSuccesfull"))
	void OnEngineRestartSuccesfull_BP(); // When puzzle completed succesfully

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnNewEngineFailure"))
	void OnNewEngineFailure_BP(); // When engine shuts down

	UFUNCTION(BlueprintCallable)
	AEngineControlPanelPuzzleTemplate *GetCurrentTemplate();

	UFUNCTION()
	void SetTask(UEDTask *task);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere)
	int PuzzleCountToSolve;

	UPROPERTY(EditAnywhere)
	int MaximumMistakes;

	UPROPERTY(EditAnywhere)
	EMistakeConcequence TooManyMistakesConcequence;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated)
	EEngineFailureType CurrentFailure;

	UPROPERTY(EditAnywhere, Replicated)
	EPuzzleColor CurrentLightColor;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent *ControlPanelMesh;

	UPROPERTY(VisibleAnywhere, Category = "Lights")
	UPointLightComponent *RedLight;

	UPROPERTY(VisibleAnywhere, Category = "Lights")
	UPointLightComponent *GreenLight;

	UPROPERTY(VisibleAnywhere, Category = "Lights")
	UPointLightComponent *YellowLight;

	UPROPERTY(VisibleAnywhere, Category = "Lights")
	UPointLightComponent *BlueLight;

	TArray<UPointLightComponent*> Lights;

	UPROPERTY(EditAnywhere, Category = "Puzzles")
	TArray<TSubclassOf<class AEngineControlPanelPuzzleTemplate>> PuzzleTemplates;

	UPROPERTY(EditAnywhere, Category = "Puzzles", Replicated)
	uint8 CurrentPuzzleTemplate;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int ButtonToPress;

	UPROPERTY(EditAnywhere, Category = "Submarine Lights")
	ALightController *LightController;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int CurrentScreenNumber;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int PuzzleSolvedCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int MistakeCounter;

private:
	UEDTask *ConnectedTask;

	FPuzzlePair FindPair(int sequenceID, const EPuzzleColor &color);

private:
	void GeneratePuzzle();
	void UpdateLights();
};
