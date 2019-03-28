#pragma once

#include "Core.h"
#include "Engine.h"
#include "Utils/Helpers.h"
#include "UnrealNetwork.h"

DECLARE_LOG_CATEGORY_EXTERN(LogED_UI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogED_Online, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogED_Voice, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogED_Game, Log, All);

const bool ED_Log = true;
const bool ED_Print = true;
const float ED_PrintTime = 30.0f;
const ELogVerbosity::Type ED_PrintVerbosity = ELogVerbosity::Log;

const FColor ED_PrintColor_LogED_UI = FColor{ 0, 255, 255 }; // Cyan
const FColor ED_PrintColor_LogED_Online = FColor{ 169, 7, 228 }; // Purple
const FColor ED_PrintColor_LogED_Voice = FColor{ 255, 255, 255 }; // White
const FColor ED_PrintColor_LogED_Game = FColor{ 255, 255, 0 }; // Yellow

#define ED_LOG(categroy, log, format, ...) \
if(ED_Log) \
{ \
    UE_LOG(categroy, log, format, __VA_ARGS__) \
} \
if(ED_Print) \
{ \
	if(static_cast<uint8>(ELogVerbosity::log) <= static_cast<uint8>(ED_PrintVerbosity)) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, ED_PrintTime, ED_PrintColor_##categroy, FString::Printf(format, __VA_ARGS__)); \
	} \
} \

#define ED_DEFAULT_SERVER_VALIDATION(class, funcName, ...) \
bool class::Server_##funcName##_Validate(__VA_ARGS__) \
{ \
	return true; \
} \
