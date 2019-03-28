#include "BrokenPipeTask.h"
#include "Tasks/Objects/BrokenPipe.h"
#include "Tasks/Objects/OilSpill.h"
#include "Game/GameModes/EDGameGameMode.h"
#include "Game/GameStates/EDGameGameState.h"
#include "EDEngineSubTask.h"

UBrokenPipeTask::UBrokenPipeTask() :BrokenPipe(nullptr)
{
	SubTaskClasses.Add(UEDEngineSubTask::StaticClass());
}

void UBrokenPipeTask::Tick(float DeltaTime)
{
	TActorIterator<ABrokenPipe> ActorItr(GetWorld());
	if (ActorItr)
		BrokenPipe = *ActorItr;

	// Trigger engine failure
	if (BrokenPipe)
	{
		if (Cast<AOilSpill>(BrokenPipe->OilSpill))
		{
			if (Cast<AOilSpill>(BrokenPipe->OilSpill)->IsOnFire)
			{
				AEDGameGameState* gameState = Cast<AEDGameGameState>(UGameplayStatics::GetGameState(GetWorld()));
				gameState->SetOxygenLevel(gameState->GetOxygenLevel() - Cast<AOilSpill>(BrokenPipe->OilSpill)->OxigenDepletionRate);
			}
		}
		if (BrokenPipe->IsComplete())
		{
			TaskManager->CompleteTask(this);
		}
	}
}

void UBrokenPipeTask::OnTaskCreated(UEDTaskManager* taskManager, const int32 numberOfPlayers)
{
	UEDTask::OnTaskCreated(taskManager, numberOfPlayers);

	//TActorIterator<ABrokenPipe> ActorItr(GetWorld());
	TArray<ABrokenPipe*> pipes = Helpers::GetAllActorsOfType<ABrokenPipe>(GetWorld());
	for (int i = 0; i < pipes.Num(); i++)
	{
		if (i == FMath::RandRange(0, pipes.Num() - 1))
		{
			if (pipes[i]->IsPipeFixed)
			{
				BrokenPipe = pipes[i];
			}
			else
			{
				NrOfBrokenPipes++;
				if (NrOfBrokenPipes < pipes.Num() - 1)
				{
					i = 0;
				}
			}
		}
	}

	// Trigger engine failure
	if (BrokenPipe)
	{
		BrokenPipe->BrakePipe();
		/*if (true)
		{
			Cast<AEDGameGameState>(UGameplayStatics::GetGameState(GetWorld()));
		}*/
	}
}

void UBrokenPipeTask::OnTaskCompleted()
{
	UEDTask::OnTaskCompleted();
}