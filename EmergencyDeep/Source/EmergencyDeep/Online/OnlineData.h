// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EmergencyDeep.h"
#include "OnlineData.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, meta = (DisplayName = "FOnlineHostData"))
struct FOnlineHostData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FText ServerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    int32 MaxPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    bool bIsLanMatch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    bool bIsPresence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName Map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString Password;

    FOnlineHostData()
    {
        ServerName = {};
        MaxPlayers = 4;
        bIsLanMatch = false;
        bIsPresence = true;
		Map = {};
		Password = {};
    }
};

USTRUCT(BlueprintType, meta = (DisplayName = "FOnlineSearchData"))
struct FOnlineSearchData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    bool bIsLanMatch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    bool bIsPresence;

    FOnlineSearchData()
    {
        bIsLanMatch = false;
        bIsPresence = true;
    }
};

USTRUCT(BlueprintType, meta = (DisplayName = "FServerBrowserData"))
struct FServerBrowserData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FText ServerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FText Players;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FText Ping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    bool PasswordProtection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString Password;

    FServerBrowserData()
    {
        ServerName = {};
        Players = {};
        Ping = {};
        PasswordProtection = {};
    }
};