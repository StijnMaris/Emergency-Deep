// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "UObject/NoExportTypes.h"
#include "EDTaskManager.generated.h"

class UEDTask;
class AEDGameGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTaskGenerated, TSubclassOf<UEDTask>, taskClass, UEDTask*, task);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTaskCompleted, TSubclassOf<UEDTask>, taskClass, UEDTask*, task);

struct TaskVariables
{
	TSubclassOf<UEDTask> TaskClass;
	float LastGenerationTime;
};

/**
 * \brief The TaskManager creates and handles all task related things. 
 */
UCLASS(BlueprintAble)
class EMERGENCYDEEP_API UEDTaskManager : public UObject
{
	GENERATED_BODY()
	
public:
    UEDTaskManager();

    void Initialize(AEDGameGameMode* gameMode);

	void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void CompleteTask(UEDTask* task);

	UFUNCTION(BlueprintCallable)
	void ForceAddTask(TSubclassOf<UEDTask> taskClass, int32 numTaskSlots = 2);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<TSubclassOf<UEDTask>> Tasks;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 TaskSlotMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GenerationStartTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GenerationUpdateTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumberOfTasksGeneratedPerUpdate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableGeneration;

	UPROPERTY(BlueprintAssignable)
	FTaskGenerated TaskGeneratedDelegate;

	UPROPERTY(BlueprintAssignable)
	FTaskCompleted TaskCompletedDelegate;

private:
    int32 GetNumberOfActiveTaskSlots();
	void GenerateTasks(int32 maxNumberOfTasksGenerated);
	TSubclassOf<UEDTask> GetRandomTask();
	int32 GetActiveTaskCount(TSubclassOf<UEDTask> taskClass);
	float GetLastGeneratedTime(TSubclassOf<UEDTask> randomTaskClass);
	void GenerateTask(int32& numberOfTaskSlotsToFind, int32& tasksGenerated, int32& playerSlotsGenerated, TSubclassOf<UEDTask> randomTaskClass);

	UPROPERTY()
    AEDGameGameMode* GameMode;

	UPROPERTY()
    TArray<UEDTask*> ActiveTasks;

	UPROPERTY()
	TArray<UEDTask*> CompletedTasks;

	UPROPERTY()
	float ElapsedTime;

	UPROPERTY()
	float LastUpdateTime;

	TArray<TaskVariables> TaskVariablesArray;
};
