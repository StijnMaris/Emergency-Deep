// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "EDBaseNetworkedPlayerController.h"
#include "EDGamePlayerController.generated.h"

/**
 *
 */
UCLASS()
class EMERGENCYDEEP_API AEDGamePlayerController : public AEDBaseNetworkedPlayerController
{
	GENERATED_BODY()
public:
	AEDGamePlayerController();

	virtual void SetupInputComponent() override;
};
