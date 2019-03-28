#include "EngineControlPanel.h"
#include "EmergencyDeep.h"
#include "EngineControlPanelPuzzleTemplate.h"
#include "Tasks/EDTask.h"
#include "Game/GameModes/EDGameGameMode.h"
#include "Tasks/EDTaskManager.h"
#include "UnrealNetwork.h"

#include "Submarine/LightController.h"

FPuzzlePair FPuzzlePair::Invalid = FPuzzlePair(false);

// Sets default values for this component's properties
AEngineControlPanel::AEngineControlPanel(const FObjectInitializer& ObjectInitializer) : PuzzleSolvedCount(0), CurrentScreenNumber(-1), PuzzleCountToSolve(4),
	TooManyMistakesConcequence(EMistakeConcequence::MC_InstantFire), CurrentPuzzleTemplate(0), Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	bReplicateMovement = false;

	ControlPanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Control Panel Mesh"));
	RootComponent = ControlPanelMesh;

	RedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Red Light"));
	RedLight->SetupAttachment(RootComponent);
	GreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Green Light"));
	GreenLight->SetupAttachment(RootComponent);
	YellowLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Yellow Light"));
	YellowLight->SetupAttachment(RootComponent);
	BlueLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Blue Light"));
	BlueLight->SetupAttachment(RootComponent);

	Lights.Add(RedLight);
	Lights.Add(GreenLight);
	Lights.Add(YellowLight);
	Lights.Add(BlueLight);
}

// Called when the game starts
void AEngineControlPanel::BeginPlay()
{
	Super::BeginPlay();

	UpdateLights();
}

// Called every frame
void AEngineControlPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		GeneratePuzzle();
		UpdateLights();
	}
}

void AEngineControlPanel::SetFailure(const EEngineFailureType &FailureType)
{
	CurrentFailure = FailureType;

	if (FailureType == EEngineFailureType::EFT_Shutdown)
	{
		if(LightController)
			LightController->SetLightState(false);

		OnNewEngineFailure_BP();
	}
}

void AEngineControlPanel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEngineControlPanel, CurrentFailure);
	DOREPLIFETIME(AEngineControlPanel, CurrentLightColor);
	DOREPLIFETIME(AEngineControlPanel, CurrentPuzzleTemplate);
	DOREPLIFETIME(AEngineControlPanel, ButtonToPress);
	DOREPLIFETIME(AEngineControlPanel, CurrentScreenNumber);
	DOREPLIFETIME(AEngineControlPanel, PuzzleSolvedCount);
	DOREPLIFETIME(AEngineControlPanel, MistakeCounter);
}

void AEngineControlPanel::ForwardAction(const FPuzzleAction &action)
{
	if (CurrentLightColor != EPuzzleColor::PC_None)
	{
		auto pair = FindPair(PuzzleSolvedCount, CurrentLightColor);
		if (!pair.bValid) return;

		if (pair.PuzzleAction.PuzzleType == EEnginePuzzleType::EFPT_TableNumberButton && (action.PuzzleType == EEnginePuzzleType::EFPT_TableNumberButton || action.PuzzleType == EEnginePuzzleType::EFPT_NumberButton))
		{
			if (action.PuzzleData == ButtonToPress)
			{
				// Correct action data
				SolveCurrentPuzzle();
				return;
			}
			else
			{
				MadeMistake(pair.MistakeConcequence);
				return;
			}
		}

		// Attempt to solve the puzzle
		if (action.PuzzleType == pair.PuzzleAction.PuzzleType)
		{
			// Correct action
			if (action.PuzzleData == pair.PuzzleAction.PuzzleData || pair.PuzzleAction.PuzzleType == EEnginePuzzleType::EFPT_LeverToggle)
			{
				// Correct action data
				SolveCurrentPuzzle();
				return;
			}
		}

		// Something was wrong!
		MadeMistake(pair.MistakeConcequence);
	}
}

void AEngineControlPanel::ResetCounter()
{
	PuzzleSolvedCount = 0;
	CurrentLightColor = EPuzzleColor::PC_None;
}

void AEngineControlPanel::ResetEngine()
{
	ResetCounter();
	CurrentLightColor = EPuzzleColor::PC_None;
	CurrentFailure = EEngineFailureType::EFT_Idle;

	if (LightController)
		LightController->SetLightState(true);

	OnEngineRestartSuccesfull_BP();
}

void AEngineControlPanel::SolveCurrentPuzzle()
{
	CurrentLightColor = EPuzzleColor::PC_None;
	++PuzzleSolvedCount;
	if (PuzzleSolvedCount >= PuzzleCountToSolve)
	{
		ResetEngine();
		PuzzleSolvedCount = 0;

		// Finish task
		if (ConnectedTask)
		{
			auto gameMode = GetWorld()->GetAuthGameMode<AEDGameGameMode>();
			if (gameMode)
			{
				gameMode->TaskManager->CompleteTask(ConnectedTask);
				ConnectedTask = nullptr;
			}
		}

		return;
	}

	OnSequenceCorrect_BP();
}

void AEngineControlPanel::MadeMistake(const EMistakeConcequence &mistake)
{
	switch (mistake)
	{
	case EMistakeConcequence::MC_None:
		// Do nothing
		break;

	case EMistakeConcequence::MC_MistakeCounter:
		++MistakeCounter;
		if (MistakeCounter >= MaximumMistakes)
		{
			if (TooManyMistakesConcequence != EMistakeConcequence::MC_MistakeCounter)
			{
				MadeMistake(TooManyMistakesConcequence);
				MistakeCounter = 0;
			}

			OnCompleteFail_BP();

			return;
		}
		break;

	case EMistakeConcequence::MC_InstantFire:
		// TODO
		return;

	case EMistakeConcequence::MC_Reset:
		// Reset the solved counter
		ResetCounter();

		OnCompleteFail_BP();
		return;
	}

	OnMadeMistake_BP();
}

AEngineControlPanelPuzzleTemplate *AEngineControlPanel::GetCurrentTemplate()
{
	return PuzzleTemplates[CurrentPuzzleTemplate].GetDefaultObject();
}

void AEngineControlPanel::SetTask(UEDTask * task)
{
	ConnectedTask = task;
}

FPuzzlePair AEngineControlPanel::FindPair(int sequenceID, const EPuzzleColor &color)
{
	for (const FPuzzlePair &puzzle : PuzzleTemplates[CurrentPuzzleTemplate].GetDefaultObject()->Puzzles)
	{
		if (puzzle.PuzzleSequence == sequenceID)
		{
			if (puzzle.LightColor == color)
			{
				return puzzle;
			}
		}
	}

	return FPuzzlePair::Invalid;
}

void AEngineControlPanel::GeneratePuzzle()
{
	if (CurrentFailure != EEngineFailureType::EFT_Idle)
	{
		if (CurrentLightColor == EPuzzleColor::PC_None)
		{
			// Generate new puzzle
			CurrentLightColor = (EPuzzleColor)FMath::RandRange(1, 4);

			// Check what puzzle we chose
			auto pair = FindPair(PuzzleSolvedCount, CurrentLightColor);
			if (!pair.bValid) return; // Pair doesnt exist

			switch (pair.PuzzleAction.PuzzleType)
			{
			case EEnginePuzzleType::EFPT_TableNumberButton:
			{
				// Generate random screen number
				auto buttonPairs = PuzzleTemplates[CurrentPuzzleTemplate].GetDefaultObject()->ButtonPairs;
				int maxNumber = buttonPairs.Num();
				CurrentScreenNumber = FMath::RandRange(0, maxNumber - 1);
				ButtonToPress = (uint8)buttonPairs[CurrentScreenNumber] + 1;
				break;
			}
			default:
				// Nothing to do
				break;
			}
		}
	}
}

void AEngineControlPanel::UpdateLights()
{
	// Safety check
	if (Lights.Num() < 4)
	{
		ED_LOG(LogED_Game, Error, TEXT(""), TEXT("AEngineControlPanel::UpdateLights > You need to have 4 lights components for the control panel to work!"));
	}

	// Turn all lights off
	for (auto light : Lights)
	{
		light->SetVisibility(false);
	}

	// Turn current light on
	if(CurrentLightColor != EPuzzleColor::PC_None)
		Lights[(uint8)CurrentLightColor - 1]->SetVisibility(true);
}
