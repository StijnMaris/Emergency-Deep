

#include "VoiceSpeakerComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Online/Voice/PlayerVoice.h"
#pragma warning(push)
#pragma warning(disable: 4005) // '???': macro redefinition
#pragma warning(disable: 4996) // '???': This function or variable may be unsafe. Consider using ??? instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#include "steam/steam_api.h"
#pragma warning(pop)

UVoiceSpeakerComponent::UVoiceSpeakerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IsOn = true;
	Volume = 5.0f;
}

void UVoiceSpeakerComponent::BeginPlay()
{
	Super::BeginPlay();

	VoiceSoundWave = NewObject<USoundWaveProcedural>();
	VoiceSoundWave->SetSampleRate(ED_VOICE_SAMPLE_RATE);
	VoiceSoundWave->NumChannels = 1;
	VoiceSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	VoiceSoundWave->SoundGroup = SOUNDGROUP_Voice;
	VoiceSoundWave->bLooping = false;
	VoiceSoundWave->bVirtualizeWhenSilent = true;
	VoiceSoundWave->Volume = Volume;
	SetSound(VoiceSoundWave);
	Play();
}

void UVoiceSpeakerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsOn && !IsPlaying()) Play();
	else if (!IsOn && IsPlaying()) Stop();
}

void UVoiceSpeakerComponent::PlayVoiceData(const TArray<uint8>& data, float volume)
{
	if(IsOn && data.Num() > 0)
	{
		// Decompress voice data
		ISteamUser* steamUser = SteamUser();
		uint32 sampleRate = ED_VOICE_SAMPLE_RATE;
		TArray<uint8> decompressedData;
		decompressedData.SetNumUninitialized(sampleRate);

		uint32 bytesWritten{};
		steamUser->DecompressVoice(
			data.GetData(), data.Num(),
			decompressedData.GetData(), decompressedData.Num(),
			&bytesWritten, sampleRate
		);

		if (static_cast<uint32>(decompressedData.Num()) <= bytesWritten)
		{
			ED_LOG(LogED_Voice, Fatal, TEXT("UVoiceSpeakerComponent::PlayVoiceData: Decompressed array size too small!"))
		}

		// Set array size to number of valid bytes of voice data
		decompressedData.SetNum(bytesWritten);

		ED_LOG(LogED_Voice, Verbose, TEXT("UVoiceSpeakerComponent::PlayVoiceData: %s queueing %i bytes of voice data."), *GetName(), decompressedData.Num())
		VoiceSoundWave->QueueAudio(decompressedData.GetData(), decompressedData.Num());
		SetVolumeMultiplier(Volume * volume);
	}
}

