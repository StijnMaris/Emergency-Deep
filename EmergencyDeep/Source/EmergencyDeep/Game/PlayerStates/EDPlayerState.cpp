// Fill out your copyright notice in the Description page of Project Settings.

#include "EDPlayerState.h"
#include "VoiceInterface.h"

AEDPlayerState::AEDPlayerState()
	: IsMuted{false}
{
}

void AEDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEDPlayerState, IsMuted);
}
