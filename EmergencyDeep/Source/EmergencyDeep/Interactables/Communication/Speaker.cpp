

#include "Speaker.h"
#include "Online/Voice/VoiceSpeakerComponent.h"
#include "Online/Voice/PlayerVoice.h"
#include "Online/Voice/VoiceRecorderComponent.h"

ASpeaker::ASpeaker()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SpeakerPositionerComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpeakerPositioner"));

	RootComponent = MeshComponent;
	SpeakerPositionerComponent->SetupAttachment(RootComponent);

	Volume = 5.0f;
}

void ASpeaker::BeginPlay()
{
	Super::BeginPlay();
}

void ASpeaker::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateComponentCount();
}

void ASpeaker::UpdateComponentCount()
{
	int32 numberOfPlayers = GetWorld()->GetGameState()->PlayerArray.Num();

	if (numberOfPlayers != SpeakerComponents.Num())
	{
		for (UVoiceSpeakerComponent* speakerComponent : SpeakerComponents)
		{
			speakerComponent->DestroyComponent();
		}
		SpeakerComponents.Empty();

		TArray<APlayerVoice*> playerVoices = Helpers::GetAllActorsOfType<APlayerVoice>(GetWorld());
		if(numberOfPlayers == playerVoices.Num())
		{
			for (int i = 0; i < numberOfPlayers; ++i)
			{
				UVoiceSpeakerComponent* speaker = NewObject<UVoiceSpeakerComponent>(this);
				speaker->PlayerNetId = playerVoices[i]->GetPlayerNetId();
				speaker->AttenuationSettings = SpeakerAttenuationSettings;
				speaker->Volume = Volume;
				speaker->RegisterComponent();
				speaker->AttachToComponent(SpeakerPositionerComponent, FAttachmentTransformRules{ EAttachmentRule::KeepRelative, false });
				SpeakerComponents.Add(speaker);
			}
		}
	}
}

void ASpeaker::PlayVoiceData(const TArray<FPlayerRecordingData>& recordingDatas)
{
	for (UVoiceSpeakerComponent* speakerComponent : SpeakerComponents)
	{
		for (const FPlayerRecordingData& recordingData : recordingDatas)
		{
			if(recordingData.PlayerId == speakerComponent->PlayerNetId)
			{
				speakerComponent->PlayVoiceData(recordingData.Voice->GetVoiceData(), recordingData.VolumeMultiplier);
			}
		}
	}
}
