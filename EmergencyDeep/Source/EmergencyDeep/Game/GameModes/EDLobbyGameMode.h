// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "EDBaseGameMode.h"
#include "EDLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDLobbyGameMode : public AEDBaseGameMode
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	
	
};
