// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "UObject/NoExportTypes.h"
#include "OnlineData.h"
#include "OnlineManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFindSessionsComplete);

class ULocalPlayer;
class AEDGameSession;

UCLASS(BlueprintAble)
class EMERGENCYDEEP_API UOnlineManager : public UObject
{
    GENERATED_BODY()

public:
    UOnlineManager();

    void Initialize();

	UFUNCTION(BlueprintCallable)
	void HostGame(APlayerController* localPlayer, const FOnlineHostData& hostData);

	UFUNCTION(BlueprintCallable)
	void FindSessions(APlayerController* localPlayer, const FOnlineSearchData& searchData);

	UFUNCTION(BlueprintCallable)
	void JoinSessions(APlayerController* localPlayer, int32 joinIndex) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FServerBrowserData>& GetServerBrowserData();

	UPROPERTY(BlueprintAssignable)
	FFindSessionsComplete OnFindSessionsCompleteDelegate;

private:
    AEDGameSession* GetGameSession() const;

    TArray<FServerBrowserData> ServerBrowserData;
};
