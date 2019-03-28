

#pragma once

#include "EmergencyDeep.h"
#include "PlayerVoice.h"
#include "GameFramework/Actor.h"
#include "PlayerVoiceManager.generated.h"

class UPlayerVoiceComponent;

UCLASS()
class EMERGENCYDEEP_API APlayerVoiceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerVoiceManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RegisterPlayer(APlayerController* owner, UPlayerVoiceComponent* voiceComponent, int32 playerNetId);
	void UnRegisterPlayer(int32 playerNetId);

	const TArray<APlayerVoice*>& GetPlayerVoices() const;
	APlayerVoice* GetVoice(int32 id);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	TArray<APlayerVoice*> PlayerVoices;

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
