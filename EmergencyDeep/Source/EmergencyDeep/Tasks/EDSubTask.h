// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "UObject/NoExportTypes.h"
#include "EDSubTask.generated.h"

class UEDTask;

/**
 * \brief SubTask has all the player number specific implementations and data. Requires a parent Task to work.
 */
UCLASS(BlueprintAble)
class EMERGENCYDEEP_API UEDSubTask : public UObject
{
	GENERATED_BODY()	
public:
    UEDSubTask();

    UFUNCTION(BlueprintImplementableEvent, Category = "SubTask", meta = (DisplayName = "On SubTask Created"))
    void OnSubTaskCreated_BP(UEDTask* parentTask);
    virtual void OnSubTaskCreated(UEDTask* parentTask);

    UFUNCTION(BlueprintImplementableEvent, Category = "SubTask", meta = (DisplayName = "On SubTask Completed"))
    void OnSubTaskCompleted_BP();
    virtual void OnSubTaskCompleted();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    int32 NumberOfPlayers;

protected:
	UPROPERTY()
	UEDTask* ParentTask;
};
