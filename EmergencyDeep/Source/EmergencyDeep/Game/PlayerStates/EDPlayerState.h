// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "EDBasePlayerState.h"
#include "EDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDPlayerState : public AEDBasePlayerState
{
	GENERATED_BODY()
public:
	AEDPlayerState();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	bool IsMuted;

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
