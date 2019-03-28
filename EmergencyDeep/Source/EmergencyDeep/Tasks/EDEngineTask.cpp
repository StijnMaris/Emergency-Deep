#include "EDEngineTask.h"
#include "EDEngineSubTask.h"
#include "Puzzles/EngineControlPanel.h"

UEDEngineTask::UEDEngineTask() : EngineControlPanel(nullptr)
{
}

void UEDEngineTask::OnTaskCreated(UEDTaskManager *taskManager, const int32 numberOfPlayers)
{
	UEDTask::OnTaskCreated(taskManager, numberOfPlayers);

	TActorIterator<AEngineControlPanel> ActorItr(GetWorld());
	if(ActorItr)
		EngineControlPanel = *ActorItr;

	// Trigger engine failure
	if (EngineControlPanel)
	{
		EngineControlPanel->CurrentPuzzleTemplate = FMath::RandRange(0, EngineControlPanel->PuzzleTemplates.Num() - 1);
		EngineControlPanel->SetFailure(EEngineFailureType::EFT_Shutdown);
		EngineControlPanel->SetTask(this);
	}
}