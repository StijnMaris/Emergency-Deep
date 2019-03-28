

#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/Actor.h"
#include "PlayerVoice.generated.h"

const int ED_VOICE_SAMPLE_RATE = 24000;

UCLASS()
class EMERGENCYDEEP_API APlayerVoice : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerVoice();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetPlayerNetId(int32 playerNetId);
	int32 GetPlayerNetId() const;

	void AddPlayerVoiceData(const TArray<uint8>& data);

	UFUNCTION(NetMulticast, Reliable)
	void Server_QueueRemoteVoiceData(const TArray<uint8>& data);

	const TArray<uint8>& GetVoiceData() const;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int32 PlayerNetId;

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ResetVoiceData();

	TArray<uint8> PlayerVoiceData;
};
