

#pragma once

#include "EmergencyDeep.h"
#include "PlayerVoice.h"
#include "Components/AudioComponent.h"
#include "PlayerVoiceComponent.generated.h"

class USoundWaveProcedural;
class APlayerVoiceManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UPlayerVoiceComponent : public UAudioComponent
{
	GENERATED_BODY()

public:	
	UPlayerVoiceComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	void SetPlayerNetID(int32 playerNetId);
	int32 GetPlayerNetID() const;

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void RecordVoice() const;
	void PlayRemoteVoices() const;
	void PlayVoice(const TArray<uint8>& data);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int32 PlayerVoiceNetID;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	APlayerVoiceManager* VoiceManager;

private:
	USoundWaveProcedural* VoiceSoundWave;

	bool HasRegistered;
};
