// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EDBaseGameMode.h"
#include "EDGameGameMode.generated.h"

class UEDTaskManager;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDGameGameMode : public AEDBaseGameMode
{
	GENERATED_BODY()
	
public:
    AEDGameGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    TSubclassOf<UEDTaskManager> TaskManagerClass;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    UEDTaskManager* TaskManager;
};


