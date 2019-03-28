// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "EDBasePlayerController.h"
#include "EDMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDMenuPlayerController : public AEDBasePlayerController
{
	GENERATED_BODY()
public:
	AEDMenuPlayerController();

	virtual void SetupInputComponent() override;
	
};
