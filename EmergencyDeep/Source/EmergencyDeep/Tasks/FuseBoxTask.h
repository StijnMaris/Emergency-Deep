

#pragma once

#include "CoreMinimal.h"
#include "Tasks/EDTask.h"
#include "FuseBoxTask.generated.h"

class ACircuitBoard;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UFuseBoxTask : public UEDTask
{
	GENERATED_BODY()
	
public:
	UFuseBoxTask();

	virtual void Tick(float DeltaTime) override;

	virtual void OnTaskCreated(UEDTaskManager* taskManager, const int32 numberOfPlayers);

	virtual void OnTaskCompleted();

private:
	ACircuitBoard* FuseBox;

	int NrOfBrokenPipes;

	
	
};
