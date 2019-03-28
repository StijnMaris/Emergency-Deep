// Fill out your copyright notice in the Description page of Project Settings.

#include "EDTaskManager.h"
#include "Game/GameModes/EDGameGameMode.h"
#include "EDTask.h"
#include "Tasks/EDSubTask.h"

UEDTaskManager::UEDTaskManager()
{
	TaskSlotMultiplier = 2.0f;
	GenerationStartTime = 120.0f;
	GenerationUpdateTime = 30.0f;
	NumberOfTasksGeneratedPerUpdate = 2;
	EnableGeneration = true;
}

void UEDTaskManager::Initialize(AEDGameGameMode* gameMode)
{
    GameMode = gameMode;
	check(gameMode)

    ED_LOG(LogED_Game, Log, TEXT("UEDTaskManager::UEDTaskManager: %i tasks registered."), Tasks.Num())
}

void UEDTaskManager::Tick(float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;

	if(ElapsedTime >= GenerationStartTime && ElapsedTime >= LastUpdateTime + GenerationUpdateTime)
	{
		LastUpdateTime = ElapsedTime;

		if(EnableGeneration)
		{
			GenerateTasks(NumberOfTasksGeneratedPerUpdate);
		}
	}

	for (UEDTask* task : ActiveTasks)
	{
		task->Tick(DeltaSeconds);
	}

	for (UEDTask* task : CompletedTasks)
	{
		ActiveTasks.Remove(task);
	}
	CompletedTasks.Empty();
}

TSubclassOf<UEDTask> UEDTaskManager::GetRandomTask()
{
	TSubclassOf<UEDTask> toGenerateTask;
	int32 randomTotalValues = 0;
	for (TSubclassOf<UEDTask> taskClass : Tasks)
	{
		UEDTask* task = taskClass.GetDefaultObject();
		randomTotalValues += task->ProbabilityMultiplier * 100.0f;
	}
	const int32 randomTaskNumber = FMath::RandRange(0, randomTotalValues);

	int32 taskEndValue = 0;
	for (TSubclassOf<UEDTask> taskClass : Tasks)
	{
		UEDTask* task = taskClass.GetDefaultObject();
		const int32 taskStartValue = taskEndValue;
		taskEndValue = taskStartValue + task->ProbabilityMultiplier * 100.0f;
		if(randomTaskNumber >= taskStartValue && randomTaskNumber < taskEndValue)
		{
			toGenerateTask = taskClass;
			break;
		}
	}
	check(toGenerateTask)
	return toGenerateTask;
}

int32 UEDTaskManager::GetActiveTaskCount(const TSubclassOf<UEDTask> taskClass)
{
	int32 activeTaskCount = 0;
	for (UEDTask* activeTask : ActiveTasks)
	{
		if (activeTask->IsA(taskClass)) 
			++activeTaskCount;
	}
	return activeTaskCount;
}

float UEDTaskManager::GetLastGeneratedTime(TSubclassOf<UEDTask> randomTaskClass)
{
	float lastGeneratedTime = 0.0f;
	bool found = false;
	for (const TaskVariables& taskVariable : TaskVariablesArray)
	{
		if(taskVariable.TaskClass.GetDefaultObject() == randomTaskClass.GetDefaultObject())
		{
			lastGeneratedTime = taskVariable.LastGenerationTime;
			found = true;
			break;
		}		
	}

	if(!found)
	{
		TaskVariables taskVars{};
		taskVars.TaskClass = randomTaskClass;
		taskVars.LastGenerationTime = 0.0f;
		TaskVariablesArray.Add(taskVars);
	}

	return lastGeneratedTime;
}

void UEDTaskManager::GenerateTask(int32& numberOfTaskSlotsToFind, int32& tasksGenerated, int32& playerSlotsGenerated, TSubclassOf<UEDTask> randomTaskClass)
{
	// Create task
	UEDTask* task = NewObject<UEDTask>(this, randomTaskClass);
	ActiveTasks.Add(task);
	++tasksGenerated;

	// Generate a random number that defines what subtask to use.
	const int32 randomPlayerNumber = FMath::RandRange(task->GetNumberOfPlayers().Min, FMath::Min(task->GetNumberOfPlayers().Max, numberOfTaskSlotsToFind));
	playerSlotsGenerated += randomPlayerNumber;
	numberOfTaskSlotsToFind -= randomPlayerNumber;

	task->OnTaskCreated(this, randomPlayerNumber);
	TaskGeneratedDelegate.Broadcast(randomTaskClass, task);
	ED_LOG(LogED_Game, Log, TEXT("UEDTaskManager::GenerateTasks: Task generated: %s, Slots: %i"), *task->GetName(), task->GetSubTask()->NumberOfPlayers);
}

void UEDTaskManager::GenerateTasks(const int32 maxNumberOfTasksGenerated)
{
    if (Tasks.Num() == 0) return;

    int32 numberOfPlayers = GameMode->GetNumPlayers() * TaskSlotMultiplier;
    int32 numberOfActiveTaskSlots = GetNumberOfActiveTaskSlots() ;
    int32 numberOfTaskSlotsToFind = numberOfPlayers - numberOfActiveTaskSlots;
    int32 tasksGenerated = 0;
    int32 playerSlotsGenerated = 0;
	const int32 maxChecks = 10;
	int32 currentChecks = 0;

	while(currentChecks <= maxChecks && numberOfTaskSlotsToFind > 0 && tasksGenerated < maxNumberOfTasksGenerated && playerSlotsGenerated < numberOfTaskSlotsToFind)
	{
		currentChecks++;

		TSubclassOf<UEDTask> randomTaskClass = GetRandomTask();
		UEDTask* task = randomTaskClass.GetDefaultObject();
		bool canGenerateTask = true;

		const int32 activeTaskCount = GetActiveTaskCount(randomTaskClass);
		canGenerateTask = task->MaxGenerated > activeTaskCount;
		if (!canGenerateTask) continue;

		const float generatedTime = GetLastGeneratedTime(randomTaskClass);
		canGenerateTask = generatedTime + task->MinTimeBetween < ElapsedTime;
		if (!canGenerateTask) continue;

		if (canGenerateTask)
		{
			GenerateTask(numberOfTaskSlotsToFind, tasksGenerated, playerSlotsGenerated, randomTaskClass);
		}
	}

	if(tasksGenerated > 0)
	{
        ED_LOG(LogED_Game, Log, TEXT("UEDTaskManager::GenerateTasks: Tasks generated: %i"), tasksGenerated);
        ED_LOG(LogED_Game, Log, TEXT("UEDTaskManager::GenerateTasks: Player slots generated: %i"), playerSlotsGenerated);
	}
}

void UEDTaskManager::CompleteTask(UEDTask* task)
{
    task->OnTaskCompleted();
    CompletedTasks.Add(task);
	TaskCompletedDelegate.Broadcast(task->StaticClass(), task);

	ED_LOG(LogED_Game, Log, TEXT("UEDTaskManager::CompleteTask: Completed task: %s"), *task->GetName())
}

void UEDTaskManager::ForceAddTask(TSubclassOf<UEDTask> taskClass, int32 numTaskSlots)
{
	int32 taskGenerated = 0;
	int32 slotsGenerated = 0;
	GenerateTask(numTaskSlots, taskGenerated, slotsGenerated, taskClass);
}

int32 UEDTaskManager::GetNumberOfActiveTaskSlots()
{
    int32 count{};

	for (UEDTask* task : ActiveTasks)
	{
        count += task->GetSubTask()->NumberOfPlayers;
	}

    return count;
}
