// Fill out your copyright notice in the Description page of Project Settings.

#include "Helpers.h"
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"

Helpers::Helpers()
{
}

Helpers::~Helpers()
{
}

bool Helpers::IsInVRMode()
{
    return GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetHMDDevice() && GEngine->XRSystem->GetHMDDevice()->IsHMDConnected();
}

bool Helpers::IsInStandaloneMode(UObject* object)
{
#if UE_BUILD_SHIPPING == 0
	if(GEngine)
	{
		if(GEngine->GetWorldFromContextObject(object, EGetWorldErrorMode::ReturnNull)->WorldType)
		{
            return GEngine->GetWorldFromContextObject(object, EGetWorldErrorMode::ReturnNull)->WorldType == EWorldType::Game;
		}
	}
    return false;
#endif

    return true;
}

bool Helpers::IsLocalPlayer(AActor* worldContext, int32 playerId)
{
	APlayerController* pc = worldContext->GetWorld()->GetFirstPlayerController();
	if(pc)
	{
		if(pc->PlayerState)
		{
			return playerId == pc->PlayerState->PlayerId;
		}
	}
	return false;
}

IOnlineVoicePtr Helpers::GetVoiceInterface()
{
	if (IOnlineSubsystem::Get())
	{
		return IOnlineSubsystem::Get()->GetVoiceInterface();
	}

	return nullptr;
}
