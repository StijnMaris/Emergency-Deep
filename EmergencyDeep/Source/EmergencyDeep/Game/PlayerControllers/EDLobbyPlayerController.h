

#pragma once

#include "EmergencyDeep.h"
#include "EDBaseNetworkedPlayerController.h"
#include "EDLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDLobbyPlayerController : public AEDBaseNetworkedPlayerController
{
	GENERATED_BODY()
public:
	AEDLobbyPlayerController();
	
	virtual void SetupInputComponent() override;
	
};
