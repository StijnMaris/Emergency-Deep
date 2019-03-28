

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "EDCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UEDCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:

protected:
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
	
	
};
