// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "UObject/NoExportTypes.h"
#include "Tasks/EDTaskManager.h"
#include "EDTask.generated.h"


class UEDSubTask;
class UEDTaskManager;

USTRUCT(BlueprintType)
struct FNumbersOfPlayers
{
	GENERATED_BODY()

    int32 Min;
    int32 Max;
};


/**
 * \brief Task is the main class of a task. It contain function and data that are not dependent on the number of players. 
 * This task contains SubTasks that contain the player number specif implementation and data.
 */
UCLASS(BlueprintAble)
class EMERGENCYDEEP_API UEDTask : public UObject
{
	GENERATED_BODY()
	
public:
    UEDTask();

	virtual void Tick(float DeltaSeconds);

    UFUNCTION(BlueprintImplementableEvent, Category = "Task", meta = (DisplayName = "OnTaskCreated"))
    void OnTaskCreated_BP(UEDTaskManager* taskManager, int32 numberOfPlayers);
    virtual void OnTaskCreated(UEDTaskManager* taskManager, int32 numberOfPlayers);

    UFUNCTION(BlueprintImplementableEvent, Category = "Task", meta = (DisplayName = "OnTaskCompleted"))
    void OnTaskCompleted_BP();
    virtual void OnTaskCompleted();

	UFUNCTION(BlueprintCallable)
    FNumbersOfPlayers GetNumberOfPlayers();

	UFUNCTION(BlueprintCallable)
    UEDSubTask* GetSubTask() const;

	UFUNCTION(BlueprintCallable)
	void Complete();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxGenerated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProbabilityMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinTimeBetween;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

protected:
    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UEDSubTask>> SubTaskClasses;

	UPROPERTY()
	UEDTaskManager* TaskManager;

	UPROPERTY()
	UEDSubTask* SubTask;
};

