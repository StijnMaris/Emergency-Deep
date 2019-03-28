// Fill out your copyright notice in the Description page of Project Settings.

#include "EDGameSession.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "VoiceDataCommon.h"
#include "Game/EDGameInstance.h"
#include "Online/OnlineManager.h"

AEDGameSession::AEDGameSession()
{
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AEDGameSession::OnCreateSessionComplete);
    OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AEDGameSession::OnStartOnlineGameComplete);

    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AEDGameSession::OnFindSessionsComplete);
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AEDGameSession::OnJoinSessionComplete);
    OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AEDGameSession::OnDestroySessionComplete);
}

void AEDGameSession::BeginPlay()
{
    Super::BeginPlay();

	// Register VOIP talker when in standalone
    if(Helpers::IsInStandaloneMode(this))
    {
        GetVoiceSession()->RegisterLocalTalker(0);
    }
}

void AEDGameSession::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AEDGameSession::HostGame(const TSharedPtr<const FUniqueNetId> userId, const FName serverDisplayName, const TSharedPtr<FOnlineSessionSettings> sessionSettings, const FOnlineHostData& hostData)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
		// Set server settings
        OnlineSessionSettings = sessionSettings;
        OnlineSessionSettings->Set("SESSION_NAME", serverDisplayName.GetPlainNameString(), EOnlineDataAdvertisementType::ViaOnlineService);
        OnlineSessionSettings->Set("PASSWORD", hostData.Password, EOnlineDataAdvertisementType::ViaOnlineService);

		// Assign delegates
        OnCreateSessionCompleteDelegateHandle = session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

        ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::HostGame: Hosting session: %s"), *serverDisplayName.GetPlainNameString());

        bool result = session->CreateSession(*userId, serverDisplayName, *sessionSettings);

        ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::HostGame: Result: %i"), result);
    }
}

void AEDGameSession::OnCreateSessionComplete(const FName sessionName, const bool bWasSuccessful)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        session->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
        if (bWasSuccessful)
        {
            OnStartSessionCompleteDelegateHandle = session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

            ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnCreateSessionComplete: Session: %s"), *sessionName.GetPlainNameString())

            bool result = session->StartSession(sessionName);

            ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnCreateSessionComplete: Result: %i"), result)
        }
    }
}

void AEDGameSession::OnStartOnlineGameComplete(FName sessionName, const bool bWasSuccessful)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);

        if (bWasSuccessful)
        {
			// Add level options.
            FString levelOptions = "listen";
            if (OnlineSessionSettings->bIsLANMatch)
            {
                levelOptions.Append("?bIsLanMatch=1");
            }

            ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnStartOnlineGameComplete: Session: %s - Traveling to new level: %s, options: %s"), *sessionName.GetPlainNameString(), *JoinLevel.GetPlainNameString(), *levelOptions)

            UGameplayStatics::OpenLevel(GetWorld(), JoinLevel, true, levelOptions);
        }
    }
}

void AEDGameSession::FindGameSessions(const TSharedPtr<const FUniqueNetId> userId, TSharedPtr<class FOnlineSessionSearch> searchSettings)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        OnlineSearchSettings = searchSettings;
        TSharedRef<FOnlineSessionSearch> searchSettingsRef = searchSettings.ToSharedRef();

        OnFindSessionsCompleteDelegateHandle = session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
        bool result = session->FindSessions(*userId, searchSettingsRef);

        ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::FindGameSessions: FindSessions: %i."), result)
    }
}

void AEDGameSession::OnFindSessionsComplete(bool wasSuccessful)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

		Cast<UEDGameInstance>(GetGameInstance())->OnlineManager->OnFindSessionsCompleteDelegate.Broadcast();

        if (OnlineSearchSettings->SearchResults.Num() > 0)
        {
            ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnFindSessionsComplete: %i sessions found."), OnlineSearchSettings->SearchResults.Num());
        }
        else
        {
            ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnFindSessionsComplete: No sessions found."))
        }
    }
}

void AEDGameSession::JoinSession(ULocalPlayer* localPlayer, APlayerController* playerController, int32 joinIndex)
{
    JoiningPlayerController = playerController;

    TSharedPtr<const FUniqueNetId> localPlayerId = localPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        if (OnlineSearchSettings->SearchResults.Num() > 0)
        {
            for (int32 i = 0; i < OnlineSearchSettings->SearchResults.Num(); i++)
            {
                if (OnlineSearchSettings->SearchResults[i].Session.OwningUserId != localPlayer->GetPreferredUniqueNetId())
                {
                    FOnlineSessionSearchResult searchResult = OnlineSearchSettings->SearchResults[i];
                    OnJoinSessionCompleteDelegateHandle = session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

                    FString sessionName = OnlineSearchSettings->SearchResults[i].Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.ToString();

                    ED_LOG(LogED_Online, Warning, TEXT("AEDGameSession::JoinSession: Joining session: %s - Name: %s"), *SessionName.GetPlainNameString(), *sessionName)

                    bool bSuccessful = session->JoinSession(*localPlayerId, SessionName, searchResult);

                    ED_LOG(LogED_Online, Warning, TEXT("AEDGameSession::JoinSession: Result: %i"), bSuccessful)
                    break;
                }
            }
        }
    }
}

void AEDGameSession::OnJoinSessionComplete(const FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnJoinSessionComplete"))

        session->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

        APlayerController* const playerController = JoiningPlayerController;

        FString travelUrl;
        if (playerController && session->GetResolvedConnectString(sessionName, travelUrl))
        {
            ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnJoinSessionComplete: Traveling to map: %s"), *travelUrl)
            playerController->ClientTravel(travelUrl, TRAVEL_Absolute);
        }
    }
}

void AEDGameSession::OnDestroySessionComplete(FName sessionName, const bool bWasSuccessful)
{
    const IOnlineSessionPtr session = GetOnlineSession();
    if (session)
    {
        ED_LOG(LogED_Online, Log, TEXT("AEDGameSession::OnDestroySessionComplete: Session: %s"), *sessionName.GetPlainNameString())

        session->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

        if (bWasSuccessful)
        {
            UGameplayStatics::OpenLevel(GetWorld(), OnLeaveSessionLevelToLoad, true);
        }
    }
}

TSharedPtr<FOnlineSessionSearch> AEDGameSession::GetSearchSettings() const
{
    return OnlineSearchSettings;
}

IOnlineSessionPtr AEDGameSession::GetOnlineSession()
{
    IOnlineSubsystem* const onlineSub = IOnlineSubsystem::Get();
    if (onlineSub)
    {
        const IOnlineSessionPtr session = onlineSub->GetSessionInterface();
        if (session)
        {
            return session;
        }
    }

    ED_LOG(LogED_Online, Warning, TEXT("AEDGameSession::GetOnlineSession: Session was nullptr."))
    return nullptr;
}

IOnlineVoicePtr AEDGameSession::GetVoiceSession()
{
    IOnlineSubsystem* const onlineSub = IOnlineSubsystem::Get();
    if (onlineSub)
    {
        const IOnlineVoicePtr voice = onlineSub->GetVoiceInterface();
        if (voice)
        {
            return voice;
        }
    }

    ED_LOG(LogED_Online, Warning, TEXT("AEDGameSession::GetVoiceSession: voice was nullptr."))
    return nullptr;
}
