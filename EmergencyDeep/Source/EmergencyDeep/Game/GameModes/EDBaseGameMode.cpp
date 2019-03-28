

#include "EDBaseGameMode.h"
#include "Player/EDPlayerCharacter.h"
#include "Game/PlayerControllers/EDBasePlayerController.h"
#include "Game/PlayerControllers/EDBaseNetworkedPlayerController.h"
#include "Game/PlayerStates/EDBasePlayerState.h"

AEDBaseGameMode::AEDBaseGameMode()
{

}

void AEDBaseGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AEDBaseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AEDBaseNetworkedPlayerController* pc = Cast<AEDBaseNetworkedPlayerController>(NewPlayer);
	if(pc)
	{
		pc->PostLogin();
	}

	AEDPlayerCharacter* character = Cast<AEDPlayerCharacter>(NewPlayer->GetCharacter());
	if(character)
	{
		character->NetOwnerId = NewPlayer->PlayerState->PlayerId;
		character->SpawnMotionControllers();		
	}
}

void AEDBaseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
