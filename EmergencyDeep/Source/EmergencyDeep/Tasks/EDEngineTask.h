#pragma once

#include "CoreMinimal.h"
#include "Tasks/EDTask.h"
#include "EDEngineTask.generated.h"

class AEngineControlPanel;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UEDEngineTask : public UEDTask
{
	GENERATED_BODY()

	UEDEngineTask();

public:
	virtual void OnTaskCreated(UEDTaskManager* taskManager, const int32 numberOfPlayers);

private:
	AEngineControlPanel *EngineControlPanel;
};
