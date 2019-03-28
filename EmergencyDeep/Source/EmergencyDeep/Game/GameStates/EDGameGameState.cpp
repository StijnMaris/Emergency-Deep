// Fill out your copyright notice in the Description page of Project Settings.

#include "EDGameGameState.h"

AEDGameGameState::AEDGameGameState()
	: OxygenLevel(100)
	, IsGameOver{ false }
	, WasGameOverEventCalled{false}
{

}

void AEDGameGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateGameOver();
}

void AEDGameGameState::UpdateGameOver()
{
	IsGameOver |= OxygenLevel <= 0.0f;
	IsGameOver |= WaterLevel >= WaterLevelGameOverValue;

	if(IsGameOver && !WasGameOverEventCalled)
	{
		WasGameOverEventCalled = true;
		OnGameOver_BP();
	}
}

int32 AEDGameGameState::GetScore() const
{
	return Score;
}

void AEDGameGameState::SetScore(int32 score)
{
	Score = score;
}

bool AEDGameGameState::GetGameOver() const
{
	return IsGameOver;
}

void AEDGameGameState::DEBUG_SetGameOver(bool gameOver)
{
	OnGameOver_BP();
}

float AEDGameGameState::GetOxygenLevel() const
{
	return OxygenLevel;
}

void AEDGameGameState::SetOxygenLevel(const float oxygenLevel)
{
	OxygenLevel = oxygenLevel;
}

float AEDGameGameState::GetWaterLevel() const
{
	return WaterLevel;
}

void AEDGameGameState::SetWaterLevel(float waterLevel)
{
	WaterLevel = waterLevel;
}

void AEDGameGameState::SetWaterLevelGameOverValue(float waterLevelGameOverValue)
{
	WaterLevelGameOverValue = waterLevelGameOverValue;
}
