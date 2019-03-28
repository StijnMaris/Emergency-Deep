

#pragma once

#include "CoreMinimal.h"
#include "Tasks/EDTask.h"
#include "BrokenPipeTask.generated.h"

class ABrokenPipe;
/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UBrokenPipeTask : public UEDTask
{
	GENERATED_BODY()

	public:
	UBrokenPipeTask();

	virtual void Tick(float DeltaTime) override;
	
	virtual void OnTaskCreated(UEDTaskManager* taskManager, const int32 numberOfPlayers);

	virtual void OnTaskCompleted();

private:
	ABrokenPipe* BrokenPipe;

	int NrOfBrokenPipes;
	
};
