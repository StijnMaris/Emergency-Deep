// Fill out your copyright notice in the Description page of Project Settings.

#include "EDSubTask.h"

UEDSubTask::UEDSubTask()
{
	NumberOfPlayers = 2;
}

void UEDSubTask::OnSubTaskCreated(UEDTask* parentTask)
{
    ParentTask = parentTask;

    OnSubTaskCreated_BP(parentTask);
}

void UEDSubTask::OnSubTaskCompleted()
{
    OnSubTaskCompleted_BP();
}