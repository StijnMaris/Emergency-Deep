// Fill out your copyright notice in the Description page of Project Settings.

#include "ClampedCounter.h"

UClampedCounter::UClampedCounter()
    : Min{}, Max{}, CurrentValue{}
{
}

UClampedCounter* UClampedCounter::Construct(const int min, const int max, const int currentValue)
{
    UClampedCounter* object = NewObject<UClampedCounter>();

    object->Min = min;
    object->Max = max;
    object->CurrentValue = currentValue;

    return object;
}

int UClampedCounter::Increment()
{
	if(CurrentValue < Max)
	{
        return ++CurrentValue;
	}
    return CurrentValue;
}

int UClampedCounter::Decrement()
{
    if (CurrentValue > Min)
    {
        --CurrentValue;
    }
    return CurrentValue;
}
