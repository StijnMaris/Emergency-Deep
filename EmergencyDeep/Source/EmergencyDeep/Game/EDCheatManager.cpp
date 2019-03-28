

#include "EDCheatManager.h"
#include "Submarine/LightController.h"

bool UEDCheatManager::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool handled = Super::ProcessConsoleExec(Cmd, Ar, Executor);
	if (!handled)
	{
		// Light Controllers
		TArray<ALightController*> lightControllers = Helpers::GetAllActorsOfType<ALightController>(GetWorld());
		if(lightControllers.Num() > 0)
		{
			handled = lightControllers[0]->ProcessConsoleExec(Cmd, Ar, Executor);
		}
		if (handled) return handled;
	}
	return handled;
}
