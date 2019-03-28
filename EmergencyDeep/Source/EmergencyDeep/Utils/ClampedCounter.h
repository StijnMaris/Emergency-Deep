// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "UObject/NoExportTypes.h"
#include "ClampedCounter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EMERGENCYDEEP_API UClampedCounter : public UObject
{
	GENERATED_BODY()

    UClampedCounter();

public:
    UFUNCTION(BlueprintCallable)
    static UClampedCounter* Construct(int min, int max, int currentValue);

    UFUNCTION(BlueprintCallable)
    int Increment();

    UFUNCTION(BlueprintCallable)
    int Decrement();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Min;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Max;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentValue;
};
