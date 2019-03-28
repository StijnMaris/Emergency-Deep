// Fill out your copyright notice in the Description page of Project Settings.

#include "EDGameGameMode.h"
#include "Tasks/EDTaskManager.h"
#include "Online/Voice/PlayerVoiceManager.h"
#include "Game/PlayerStates/EDPlayerState.h"
#include "Game/PlayerControllers/EDGamePlayerController.h"
#include "Player/EDPlayerCharacter.h"

AEDGameGameMode::AEDGameGameMode()
	: TaskManager{nullptr}
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AEDGameGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (TaskManagerClass)
    {
        TaskManager = NewObject<UEDTaskManager>(this, TaskManagerClass);
        TaskManager->Initialize(this);
    }

	AActor* playerVoiceManager = GetWorld()->SpawnActor(APlayerVoiceManager::StaticClass());
	playerVoiceManager->SetOwner(GetOwner());
}

void AEDGameGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	TaskManager->Tick(DeltaSeconds);
}
