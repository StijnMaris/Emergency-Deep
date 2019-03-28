// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "Engine/GameInstance.h"
#include "EDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UEDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UEDGameInstance();

    UFUNCTION(BlueprintCallable)
    void Initialize();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Managers Classes")
    TSubclassOf<class UOnlineManager> OnlineManagerClass;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Managers")
    UOnlineManager* OnlineManager;
};
