// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineManager.h"
#include "Online/EDGameSession.h"
#include "Engine/LocalPlayer.h"

UOnlineManager::UOnlineManager()
{
}

void UOnlineManager::Initialize()
{
}

void UOnlineManager::HostGame(APlayerController* localPlayer, const FOnlineHostData& hostData)
{
    AEDGameSession* const gameSession = GetGameSession();
    if (gameSession)
    {
        TSharedPtr<const FUniqueNetId> localPlayerId = localPlayer->GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

        TSharedPtr<class FOnlineSessionSettings> sessionSettings = MakeShareable(new FOnlineSessionSettings());
        sessionSettings->bIsLANMatch = hostData.bIsLanMatch;
        sessionSettings->bUsesPresence = hostData.bIsPresence;
        sessionSettings->NumPublicConnections = hostData.MaxPlayers;
        sessionSettings->bShouldAdvertise = true;
        sessionSettings->bAllowJoinInProgress = true;
        sessionSettings->bAllowInvites = true;
        sessionSettings->bAllowJoinViaPresence = true;
        sessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

		gameSession->JoinLevel = hostData.Map;

        FName name = FName{ *hostData.ServerName.ToString() };
        gameSession->HostGame(localPlayerId, name, sessionSettings, hostData);
    }
}

void UOnlineManager::FindSessions(APlayerController* localPlayer, const FOnlineSearchData& searchData)
{
    AEDGameSession* const gameSession = GetGameSession();
    if (gameSession)
    {
        TSharedPtr<const FUniqueNetId> localPlayerId = localPlayer->GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

        TSharedPtr<class FOnlineSessionSearch> sessionSettings = MakeShareable(new FOnlineSessionSearch());
        sessionSettings->bIsLanQuery = searchData.bIsLanMatch;
        sessionSettings->MaxSearchResults = 20000000;
        sessionSettings->PingBucketSize = 50;
        sessionSettings->QuerySettings.Set(SEARCH_PRESENCE, searchData.bIsPresence, EOnlineComparisonOp::Equals);

        gameSession->FindGameSessions(localPlayerId, sessionSettings);
    }
}

void UOnlineManager::JoinSessions(APlayerController* localPlayer, const int32 joinIndex) const
{
    AEDGameSession* const gameSession = GetGameSession();
    if (gameSession)
    {
        TSharedPtr<FOnlineSessionSearch> searchSettings = gameSession->GetSearchSettings();
        TSharedPtr<const FUniqueNetId> localPlayerId = localPlayer->GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

        if (searchSettings->SearchResults.Num() > 0)
        {
            for (int32 i = 0; i < searchSettings->SearchResults.Num(); i++)
            {
                if (searchSettings->SearchResults[i].Session.OwningUserId != localPlayer->GetLocalPlayer()->GetPreferredUniqueNetId())
                {
                    FOnlineSessionSearchResult searchResult = searchSettings->SearchResults[i];
                    gameSession->JoinSession(localPlayer->GetLocalPlayer(), localPlayer->GetLocalPlayer()->GetPlayerController(GetWorld()), joinIndex);
                    break;
                }
            }
        }
    }
}

const TArray<FServerBrowserData>& UOnlineManager::GetServerBrowserData()
{
    AEDGameSession* const gameSession = GetGameSession();
    if (gameSession)
    {
        TSharedPtr<FOnlineSessionSearch> searchSettings = gameSession->GetSearchSettings();

        if (searchSettings.IsValid())
        {
            ServerBrowserData.SetNum(searchSettings->SearchResults.Num());

            for (int32 i = 0; i < searchSettings->SearchResults.Num(); i++)
            {
                FString serverName;
                bool result = searchSettings->SearchResults[i].Session.SessionSettings.Get("SESSION_NAME", serverName);
				FString password;
                result = searchSettings->SearchResults[i].Session.SessionSettings.Get("PASSWORD", password);
                ServerBrowserData[i].ServerName = FText::FromString(serverName);
                ServerBrowserData[i].Password = password;
                ServerBrowserData[i].PasswordProtection = password.IsEmpty();

                int32 maxPlayers = searchSettings->SearchResults[i].Session.SessionSettings.NumPublicConnections;
                int32 openSlots = searchSettings->SearchResults[i].Session.NumOpenPublicConnections;
                FFormatNamedArguments arguments;
                arguments.Add(TEXT("CurrentPlayers"), FText::AsNumber(maxPlayers - openSlots));
                arguments.Add(TEXT("MaxPlayers"), FText::AsNumber(maxPlayers));
                FText players = FText::Format(NSLOCTEXT("UOnlineManager", "Players", "{CurrentPlayers} / {MaxPlayers}"), arguments);

                ServerBrowserData[i].Players = players;
                ServerBrowserData[i].Ping = FText::AsNumber(searchSettings->SearchResults[i].PingInMs);
            }
        }
        else
        {
            ServerBrowserData.SetNum(0);
        }
    }

    return ServerBrowserData;
}

AEDGameSession* UOnlineManager::GetGameSession() const
{
    UWorld* const world = GetWorld();
    if (world)
    {
        AGameModeBase* game = world->GetAuthGameMode();
        if (game)
        {
            return Cast<AEDGameSession>(game->GameSession);
        }
    }

    ED_LOG(LogED_Online, Warning, TEXT("UOnlineManager::GetGameSession: AEDGameSession was nullptr."))
        return nullptr;
}
