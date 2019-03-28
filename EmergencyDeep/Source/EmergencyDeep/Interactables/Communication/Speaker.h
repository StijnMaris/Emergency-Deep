

#pragma once

#include "EmergencyDeep.h"
#include "Interactables/BaseInteractable.h"
#include "Online/Voice/VoiceRecorderComponent.h"
#include "Speaker.generated.h"

class UVoiceSpeakerComponent;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API ASpeaker : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	ASpeaker();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	void PlayVoiceData(const TArray<FPlayerRecordingData>& recordingDatas);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* SpeakerPositionerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UVoiceSpeakerComponent*> SpeakerComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundAttenuation* SpeakerAttenuationSettings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Volume;

private:
	/**
	 * \brief Update the number of components based on the number of players connected to the session.
	 */
	void UpdateComponentCount();

	TArray<FPlayerRecordingData> RecordingData;
};
