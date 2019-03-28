// Fill out your copyright notice in the Description page of Project Settings.

#include "EDLobbyGameMode.h"
#include "Online/Voice/PlayerVoiceManager.h"

void AEDLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	AActor* playerVoiceManager = GetWorld()->SpawnActor(APlayerVoiceManager::StaticClass());
	playerVoiceManager->SetOwner(GetOwner());
}
