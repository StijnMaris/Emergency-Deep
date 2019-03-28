// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "EDBaseGameState.h"
#include "EDGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDGameGameState : public AEDBaseGameState
{
	GENERATED_BODY()
	
public:
	AEDGameGameState();

	void Tick(float DeltaSeconds) override;

	void UpdateGameOver();

	UFUNCTION(BlueprintCallable)
	float GetOxygenLevel() const;

	UFUNCTION(BlueprintCallable)
	void SetOxygenLevel(float oxygenLevel);

	UFUNCTION(BlueprintCallable)
	float GetWaterLevel() const;

	UFUNCTION(BlueprintCallable)
	void SetWaterLevel(float waterLevel);

	UFUNCTION(BlueprintCallable)
	void SetWaterLevelGameOverValue(float waterLevelGameOverValue);

	UFUNCTION(BlueprintCallable)
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable)
	void SetScore(int32 score);

	UFUNCTION(BlueprintCallable)
	bool GetGameOver() const;

	UFUNCTION(BlueprintCallable)
	void DEBUG_SetGameOver(bool gameOver);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGameOver"))
	void OnGameOver_BP();

private:
	float OxygenLevel;
	float WaterLevel;
	float WaterLevelGameOverValue;

	int32 Score;

	bool IsGameOver;

	bool WasGameOverEventCalled;
};
