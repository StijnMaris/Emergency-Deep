// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "OnlineSubsystem.h"

/**
 * 
 */
class EMERGENCYDEEP_API Helpers
{
public:
	Helpers();
	~Helpers();

	UFUNCTION(BlueprintCallable)
    static bool IsInVRMode();

	UFUNCTION(BlueprintCallable)
    static bool IsInStandaloneMode(UObject* object);

	UFUNCTION(BlueprintCallable)
	static bool IsLocalPlayer(AActor* worldContext, int32 playerId);

	template<typename ActorType>
	static TArray<ActorType*> GetAllActorsOfType(UWorld* world);

	static IOnlineVoicePtr GetVoiceInterface();
};

template <typename ActorType>
TArray<ActorType*> Helpers::GetAllActorsOfType(UWorld* world)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(world, ActorType::StaticClass(), actors);
	TArray<ActorType*> castedActors{};
	for (AActor* element : actors)
	{
		castedActors.Add(Cast<ActorType>(element));
	}
	return castedActors;
}
