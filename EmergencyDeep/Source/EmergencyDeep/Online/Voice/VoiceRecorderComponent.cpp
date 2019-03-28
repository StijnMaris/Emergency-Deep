

#include "VoiceRecorderComponent.h"
#include "Online/Voice/PlayerVoice.h"
#include "Online/Voice/PlayerVoiceComponent.h"
#include "Player/EDPlayerCharacter.h"

UVoiceRecorderComponent::UVoiceRecorderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVoiceRecorderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVoiceRecorderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecordVoices();
}

void UVoiceRecorderComponent::RecordVoices()
{
	RecordedData = {};

	if (IsRecording)
	{
		const TArray<APlayerVoice*> playerVoices = Helpers::GetAllActorsOfType<APlayerVoice>(GetWorld());

		RecordedData = GeneratePlayerRecordData(playerVoices);

		RecordedData.RemoveAll([](const FPlayerRecordingData& data) { return !data.IsValid(); });
	}
}


void UVoiceRecorderComponent::SetIsRecording(bool isRecording)
{
	ED_LOG(LogED_Voice, Verbose, TEXT("UVoiceRecorderComponent::SetIsRecording: %i"), isRecording)
	IsRecording = isRecording;
}

bool UVoiceRecorderComponent::GetIsRecording() const
{
	return IsRecording;
}

const TArray<FPlayerRecordingData>& UVoiceRecorderComponent::GetRecordedData() const
{
	return RecordedData;
}

TArray<FPlayerRecordingData> UVoiceRecorderComponent::GeneratePlayerRecordData(const TArray<APlayerVoice*>& playerVoices) const
{
	TArray<FPlayerRecordingData> playerRecordingDatas{};
	TArray<AEDPlayerCharacter*> players = Helpers::GetAllActorsOfType<AEDPlayerCharacter>(GetWorld());

	for (APlayerVoice* playerVoice : playerVoices)
	{
		FPlayerRecordingData distanceData{};
		distanceData.PlayerId = playerVoice->GetPlayerNetId();
		distanceData.Voice = playerVoice;

		// Calculate player distance to recording component
		for (AEDPlayerCharacter* player : players)
		{
			if(player->PlayerVoiceComponent->GetPlayerNetID() == distanceData.PlayerId)
			{
				distanceData.Distance = FVector::Dist(GetComponentLocation(), player->PlayerVoiceComponent->GetComponentLocation());
			}
		}

		playerRecordingDatas.Add(distanceData);
	}

	// Calculate volume multiplier based on distance
	for (FPlayerRecordingData& playerDistance : playerRecordingDatas)
	{
		playerDistance.VolumeMultiplier = 1.0f - playerDistance.Distance / MaxRecordingDistance;
	}

	// Sort all recordings based on distance
	playerRecordingDatas.Sort();

	return playerRecordingDatas;
}

