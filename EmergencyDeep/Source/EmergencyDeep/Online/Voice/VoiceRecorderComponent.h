

#pragma once

#include "EmergencyDeep.h"
#include "Components/SceneComponent.h"
#include "VoiceRecorderComponent.generated.h"

class APlayerVoice;

USTRUCT()
struct FPlayerRecordingData
{
	GENERATED_BODY()

	int32 PlayerId;
	float Distance;
	APlayerVoice* Voice;
	float VolumeMultiplier;

	bool operator<(const FPlayerRecordingData& other) const
	{
		return Distance < other.Distance;
	}

	bool IsValid() const
	{
		return VolumeMultiplier >= 0.0f && VolumeMultiplier <= 1.0f && Voice;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UVoiceRecorderComponent : public USceneComponent
{
	GENERATED_BODY()
public:	
	UVoiceRecorderComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetIsRecording(bool isRecording);

	UFUNCTION(BlueprintCallable)
	bool GetIsRecording() const;

	const TArray<FPlayerRecordingData>& GetRecordedData() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxRecordingDistance;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsRecording;

private:
	void RecordVoices();
	TArray<FPlayerRecordingData> GeneratePlayerRecordData(const TArray<APlayerVoice*>& playerVoices) const;

	TArray<FPlayerRecordingData> RecordedData;
};
