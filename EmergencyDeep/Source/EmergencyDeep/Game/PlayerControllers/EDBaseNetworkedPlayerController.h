

#pragma once

#include "CoreMinimal.h"
#include "EDBasePlayerController.h"
#include "EDBaseNetworkedPlayerController.generated.h"

class UPlayerVoiceComponent;
class AEDPlayerState;
class ASpeakerRecorder;
class AEDPlayerHand;

/**
 * brief Player controller base that is used for a networked levels. eg. Lobby and Game.
 */
UCLASS()
class EMERGENCYDEEP_API AEDBaseNetworkedPlayerController : public AEDBasePlayerController
{
	GENERATED_BODY()
public:
	AEDBaseNetworkedPlayerController();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void SetupInputComponent() override;
	void RefreshNames() const;
	void PostLogin();
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Voice
	void Mute();


	//Voice
	void RegisterPlayer(UPlayerVoiceComponent* playerVoiceComponent);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RegisterPlayer(UPlayerVoiceComponent* playerVoiceComponent);

	void AddVoiceData(const TArray<uint8>& data);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddVoiceData(const TArray<uint8>& data);

	// Portable radio
	void PlayPortableRadio(APortableRadio* radio);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayPortableRadio(APortableRadio* radio);

	void StopPortableRadio(APortableRadio* radio);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopPortableRadio(APortableRadio* radio);

	// Speaker recorder
	void StartSpeakerRecorder(ASpeakerRecorder* recorder);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartSpeakerRecorder(ASpeakerRecorder* recorder);

	void StopSpeakerRecorder(ASpeakerRecorder* recorder);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopSpeakerRecorder(ASpeakerRecorder* recorder);

	// Destroy hands
	void DestroyHands(AEDPlayerHand* leftHand, AEDPlayerHand* rightHand);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestroyHands(AEDPlayerHand* leftHand, AEDPlayerHand* rightHand);
};
