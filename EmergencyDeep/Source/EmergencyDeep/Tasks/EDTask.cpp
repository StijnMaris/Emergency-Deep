// Fill out your copyright notice in the Description page of Project Settings.

#include "EDTask.h"
#include "EDSubTask.h"

UEDTask::UEDTask()
{
	MaxGenerated = 1;
	ProbabilityMultiplier = 0.5f;
	MinTimeBetween = 0.0f;
	StartTime = 0.0f;
}

void UEDTask::Tick(float DeltaSeconds)
{

}

void UEDTask::OnTaskCreated(UEDTaskManager* taskManager, const int32 numberOfPlayers)
{
    TaskManager = taskManager;

    OnTaskCreated_BP(taskManager, numberOfPlayers);

	for (TSubclassOf<UEDSubTask> subTaskClass : SubTaskClasses)
	{
		if(subTaskClass.GetDefaultObject()->NumberOfPlayers == numberOfPlayers)
		{
            SubTask = NewObject<UEDSubTask>(this, subTaskClass);
            SubTask->OnSubTaskCreated(this);
            break;
		}
	}
}

void UEDTask::OnTaskCompleted()
{
    OnTaskCompleted_BP();

    SubTask->OnSubTaskCompleted();
}

FNumbersOfPlayers UEDTask::GetNumberOfPlayers()
{
    FNumbersOfPlayers minMax{100,0};

	for (TSubclassOf<UEDSubTask> subTaskClass : SubTaskClasses)
	{
		const int32 numberOfPlayers = subTaskClass.GetDefaultObject()->NumberOfPlayers;
        if (minMax.Min > numberOfPlayers) minMax.Min = numberOfPlayers;
        if (minMax.Max < numberOfPlayers) minMax.Max = numberOfPlayers;
	}

    return minMax;
}

UEDSubTask* UEDTask::GetSubTask() const
{
    return SubTask;
}

void UEDTask::Complete()
{
	TaskManager->CompleteTask(this);
}
