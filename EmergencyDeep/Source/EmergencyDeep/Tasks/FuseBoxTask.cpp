#include "FuseBoxTask.h"
#include "CircuitBoard.h"
UFuseBoxTask::UFuseBoxTask()
{
	MaxGenerated = 5;
}

void UFuseBoxTask::Tick(float DeltaTime)
{

	// Trigger engine failure
	if (FuseBox->KickStarted)
	{
		TaskManager->CompleteTask(this);
	}

}

void UFuseBoxTask::OnTaskCreated(UEDTaskManager * taskManager, const int32 numberOfPlayers)
{
	UEDTask::OnTaskCreated(taskManager, numberOfPlayers);

	TArray<ACircuitBoard*> fuseBoxes = Helpers::GetAllActorsOfType<ACircuitBoard>(GetWorld());
	MaxGenerated = fuseBoxes.Num() / 2.0f;

	int i;
	bool found = false;
	int tries = 0;
	do
	{
		i = FMath::RandRange(0,fuseBoxes.Num() - 1);
		if (fuseBoxes[i]->KickStarted)
		{
			FuseBox = fuseBoxes[i];
			found = true;
		}
		tries++;

	} while (found == false || tries > fuseBoxes.Num());

	//Break fusebox
	if (found)
	{
		FuseBox->BreakFuseBox(false);
		for (int i = 0; i < FuseBox->LinkedCirquits.Num(); i++)
		{
			FuseBox->LinkedCirquits[i]->BreakFuseBox(false);
		}
	}
	else
	{
		TaskManager->CompleteTask(this);
	}

}

void UFuseBoxTask::OnTaskCompleted()
{
	UEDTask::OnTaskCompleted();
}
