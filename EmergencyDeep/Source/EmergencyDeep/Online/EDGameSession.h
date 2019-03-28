// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/GameSession.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"
#include "VoiceInterface.h"
#include "EDGameSession.generated.h"

struct FOnlineHostData;
class ULocalPlayer;
/**
 *
 */

UCLASS(Blueprintable)
class EMERGENCYDEEP_API AEDGameSession : public AGameSession
{
    GENERATED_BODY()

public:
    AEDGameSession();

    void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

    void HostGame(TSharedPtr<const FUniqueNetId> userId, FName serverDisplayName, TSharedPtr<class FOnlineSessionSettings> sessionSettings, const FOnlineHostData& hostData);

    void FindGameSessions(TSharedPtr<const FUniqueNetId> userId, TSharedPtr<class FOnlineSessionSearch> searchSettings);

    void JoinSession(ULocalPlayer* localPlayer, APlayerController* playerController, int32 joinIndex);

    TSharedPtr<FOnlineSessionSearch> GetSearchSettings() const;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName JoinLevel = FName{ "L_Submarine" };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName OnLeaveSessionLevelToLoad = FName{ "L_MainMenu" };

private:
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName sessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool wasSuccessful);

	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	void OnDestroySessionComplete(FName sessionName, bool bWasSuccessful);

    // On create session complete
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;

    // On start session complete
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
    FDelegateHandle OnStartSessionCompleteDelegateHandle;

    // On find Sessions complete
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;

    // On join sessions complete
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;

    // On destroy session complete
    FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
    FDelegateHandle OnDestroySessionCompleteDelegateHandle;

    TSharedPtr<FOnlineSessionSettings> OnlineSessionSettings;
    TSharedPtr<FOnlineSessionSearch> OnlineSearchSettings;
    APlayerController* JoiningPlayerController;

    static IOnlineSessionPtr GetOnlineSession();
    static IOnlineVoicePtr GetVoiceSession();
};
