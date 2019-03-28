

#pragma once

#include "EmergencyDeep.h"
#include "Interactables/BaseInteractable.h"
#include "SpeakerRecorder.generated.h"

class UVoiceRecorderComponent;
class ASpeaker;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API ASpeakerRecorder : public ABaseInteractable
{
	GENERATED_BODY()
public:
	ASpeakerRecorder();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void ButtonPressed();
	void ButtonReleased();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UVoiceRecorderComponent* RecorderComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UChildActorComponent* PushToTalkButton;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxRecordingDistance;

private:
	UFUNCTION()
	void OnButtonPressed();

	UFUNCTION()
	void OnButtonReleased();

	TArray<ASpeaker*> Speakers;
};
