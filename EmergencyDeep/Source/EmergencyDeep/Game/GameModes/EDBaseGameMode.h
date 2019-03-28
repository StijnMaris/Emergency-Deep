

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EDBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AEDBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AEDBaseGameMode();

	void BeginPlay() override;
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;
};
