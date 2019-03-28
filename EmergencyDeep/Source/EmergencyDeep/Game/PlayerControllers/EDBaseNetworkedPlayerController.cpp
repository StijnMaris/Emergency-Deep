

#include "EDBaseNetworkedPlayerController.h"
#include "Online/EDGameSession.h"
#include "Player/EDPlayerCharacter.h"
#include "Player/EDPlayerHand.h"
#include "Game/PlayerStates/EDPlayerState.h"
#include "Online/Voice/PlayerVoice.h"
#include "Online/Voice/PlayerVoiceManager.h"
#include "Props/PortableRadio.h"
#include "Interactables/Communication/SpeakerRecorder.h"

AEDBaseNetworkedPlayerController::AEDBaseNetworkedPlayerController()
{

}

void AEDBaseNetworkedPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AEDBaseNetworkedPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO: make this an one time event.
	RefreshNames();
}

void AEDBaseNetworkedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Mute", IE_Pressed, this, &AEDBaseNetworkedPlayerController::Mute);
}

void AEDBaseNetworkedPlayerController::RefreshNames() const
{
	TArray<AEDPlayerCharacter*> characters = Helpers::GetAllActorsOfType<AEDPlayerCharacter>(GetWorld());
	for (AEDPlayerCharacter* character : characters)
	{
		if (character->PlayerState)
		{
			character->PlayerNameComponent->SetText(FText::FromString(character->PlayerState->GetPlayerName()));
		}
	}
}

void AEDBaseNetworkedPlayerController::PostLogin()
{
	if(Helpers::GetVoiceInterface())
	{
		Helpers::GetVoiceInterface()->StartNetworkedVoice(0);
	}

	RefreshNames();
}

void AEDBaseNetworkedPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEDBaseNetworkedPlayerController::Mute()
{
	if (GetEDPlayerState()->IsMuted)
	{
		GetEDPlayerState()->IsMuted = false;
		Helpers::GetVoiceInterface()->StartNetworkedVoice(0);
		ED_LOG(LogED_Voice, Log, TEXT("Unmuted"))
	}
	else
	{
		GetEDPlayerState()->IsMuted = true;
		Helpers::GetVoiceInterface()->StopNetworkedVoice(0);
		ED_LOG(LogED_Voice, Log, TEXT("Muted"))
	}
}

void AEDBaseNetworkedPlayerController::RegisterPlayer(UPlayerVoiceComponent* playerVoiceComponent)
{
	Server_RegisterPlayer(playerVoiceComponent);
}

void AEDBaseNetworkedPlayerController::Server_RegisterPlayer_Implementation(UPlayerVoiceComponent* playerVoiceComponent)
{
	TArray<APlayerVoiceManager*> managers = Helpers::GetAllActorsOfType<APlayerVoiceManager>(GetWorld());
	if (managers.Num() > 0)
	{
		Cast<APlayerVoiceManager>(managers[0])->RegisterPlayer(this, playerVoiceComponent, PlayerState->PlayerId);
	}
	else
	{
		ED_LOG(LogED_Online, Warning, TEXT("AEDBaseNetworkedPlayerController::RegisterPlayer: No voice manager found."))
	}
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, RegisterPlayer, UPlayerVoiceComponent* playerVoiceComponent)

void AEDBaseNetworkedPlayerController::AddVoiceData(const TArray<uint8>& data)
{
	Server_AddVoiceData(data);
}

void AEDBaseNetworkedPlayerController::Server_AddVoiceData_Implementation(const TArray<uint8>& data)
{
	TArray<APlayerVoice*> voices = Helpers::GetAllActorsOfType<APlayerVoice>(GetWorld());
	for (APlayerVoice* voice : voices)
	{
		if (voice->GetPlayerNetId() == PlayerState->PlayerId)
		{
			voice->AddPlayerVoiceData(data);
			return;
		}
	}
	ED_LOG(LogED_Online, Warning, TEXT("AEDBaseNetworkedPlayerController::AddVoiceData: No valid voice actor was found."))
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, AddVoiceData, const TArray<uint8>& data)

void AEDBaseNetworkedPlayerController::PlayPortableRadio(APortableRadio* radio)
{
	Server_PlayPortableRadio(radio);
}

void AEDBaseNetworkedPlayerController::Server_PlayPortableRadio_Implementation(APortableRadio* radio)
{
	radio->Play();
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, PlayPortableRadio, APortableRadio* radio)

void AEDBaseNetworkedPlayerController::StopPortableRadio(APortableRadio* radio)
{
	Server_StopPortableRadio(radio);
}

void AEDBaseNetworkedPlayerController::Server_StopPortableRadio_Implementation(APortableRadio* radio)
{
	radio->Stop();
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, StopPortableRadio, APortableRadio* radio)

void AEDBaseNetworkedPlayerController::StartSpeakerRecorder(ASpeakerRecorder* recorder)
{
	Server_StartSpeakerRecorder(recorder);
}

void AEDBaseNetworkedPlayerController::Server_StartSpeakerRecorder_Implementation(ASpeakerRecorder* recorder)
{
	recorder->ButtonPressed();
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, StartSpeakerRecorder, ASpeakerRecorder* recorder)

void AEDBaseNetworkedPlayerController::StopSpeakerRecorder(ASpeakerRecorder* recorder)
{
	Server_StopSpeakerRecorder(recorder);
}

void AEDBaseNetworkedPlayerController::Server_StopSpeakerRecorder_Implementation(ASpeakerRecorder* recorder)
{
	recorder->ButtonReleased();
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, StopSpeakerRecorder, ASpeakerRecorder* recorder)

void AEDBaseNetworkedPlayerController::DestroyHands(AEDPlayerHand* leftHand, AEDPlayerHand* rightHand)
{
	Server_DestroyHands(leftHand, rightHand);
}

void AEDBaseNetworkedPlayerController::Server_DestroyHands_Implementation(AEDPlayerHand* leftHand, AEDPlayerHand* rightHand)
{
	leftHand->Destroy(true, true);
	rightHand->Destroy(true, true);
}

ED_DEFAULT_SERVER_VALIDATION(AEDBaseNetworkedPlayerController, DestroyHands, AEDPlayerHand* leftHand, AEDPlayerHand* rightHand)
