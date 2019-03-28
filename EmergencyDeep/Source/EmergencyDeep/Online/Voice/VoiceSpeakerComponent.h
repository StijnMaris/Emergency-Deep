

#pragma once

#include "EmergencyDeep.h"
#include "Components/AudioComponent.h"
#include "VoiceSpeakerComponent.generated.h"

class USoundWaveProcedural;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UVoiceSpeakerComponent : public UAudioComponent
{
	GENERATED_BODY()

public:	
	UVoiceSpeakerComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayVoiceData(const TArray<uint8>& data, float volume);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Volume;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 PlayerNetId;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsOn;

private:
	USoundWaveProcedural* VoiceSoundWave;
};
