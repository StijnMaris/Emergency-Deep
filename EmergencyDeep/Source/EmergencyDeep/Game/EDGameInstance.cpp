// Fill out your copyright notice in the Description page of Project Settings.

#include "EDGameInstance.h"
#include "Online/OnlineManager.h"

UEDGameInstance::UEDGameInstance()
{
}

void UEDGameInstance::Initialize()
{
    OnlineManager = NewObject<UOnlineManager>(this, OnlineManagerClass);
    OnlineManager->Initialize();
}


