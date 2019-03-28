#include "PlayerVoiceComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Online/Voice/PlayerVoiceManager.h"
#include "OnlineSubsystem.h"
#include "VoiceInterface.h"
#include "VoiceDataCommon.h"
#include "PlayerVoice.h"
#include "Game/PlayerControllers/EDGamePlayerController.h"
#pragma warning(push)
#pragma warning(disable: 4005) // '???': macro redefinition
#pragma warning(disable: 4996) // '???': This function or variable may be unsafe. Consider using ??? instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#include "steam/steam_api.h"
#include "Player/EDPlayerCharacter.h"
#pragma warning(pop)

UPlayerVoiceComponent::UPlayerVoiceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bReplicates = true;

	HasRegistered = false;
}

void UPlayerVoiceComponent::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PrePhysics);

	// Sound wave to play all voice data for this player.
	VoiceSoundWave = NewObject<USoundWaveProcedural>();
	VoiceSoundWave->SetSampleRate(ED_VOICE_SAMPLE_RATE);
	VoiceSoundWave->NumChannels = 1;
	VoiceSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	VoiceSoundWave->SoundGroup = SOUNDGROUP_Voice;
	VoiceSoundWave->bLooping = false;
	VoiceSoundWave->bVirtualizeWhenSilent = true;
	VoiceSoundWave->Volume = 5.0f;
	SetSound(VoiceSoundWave);
	Play();
}

void UPlayerVoiceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Check for the voice manager in the level and register if this is the local player.
	if(!HasRegistered)
	{
		APlayerController* localPc = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
		if (GetOwner() == localPc->AcknowledgedPawn)
		{
			TArray<APlayerVoiceManager*> voiceManagers = Helpers::GetAllActorsOfType<APlayerVoiceManager>(GetWorld());
			if (voiceManagers.Num() > 0)
			{
				VoiceManager = voiceManagers[0];
				Cast<AEDBaseNetworkedPlayerController>(localPc)->RegisterPlayer(this);
				HasRegistered = true;
			}
		}
	}

	// Check if this is the local player.
	APlayerController* localPc = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
	if(VoiceManager && GetOwner() == localPc->AcknowledgedPawn)
	{
		RecordVoice();
		PlayRemoteVoices();
	}
}

void UPlayerVoiceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UPlayerVoiceComponent::SetPlayerNetID(int32 playerNetId)
{
	PlayerVoiceNetID = playerNetId;
}

int32 UPlayerVoiceComponent::GetPlayerNetID() const
{
	return PlayerVoiceNetID;
}

void UPlayerVoiceComponent::RecordVoice() const
{
	if(SteamUser() && Helpers::GetVoiceInterface() && Helpers::GetVoiceInterface()->GetLocalPacket(0).IsValid())
	{
		// Write all packet data to archive. i cannot get the data otherwise.
		FBitWriter writer{ 0, true };
		IOnlineSubsystem::Get()->GetVoiceInterface()->GetLocalPacket(0)->Serialize(writer);
		FBitReader reader{ writer.GetData(), writer.GetNumBits() };
		IOnlineSubsystem::Get()->GetVoiceInterface()->ClearVoicePackets();

		// Check if packet was valid.
		if (writer.GetNumBits())
		{
			// Read all data from archive
			uint64 sender{};
			reader << sender;

			uint16 length{};
			reader << length;

			TArray<uint8> compressedData;
			compressedData.SetNumUninitialized(length);
			reader.Serialize(compressedData.GetData(), length);

			// Send voice data to the voice manager.
			if (VoiceManager->GetVoice(PlayerVoiceNetID))
			{
				ED_LOG(LogED_Voice, Verbose, TEXT("UPlayerVoiceComponent::RecordVoiceSteam: Recorded %i compressed audio bytes."), compressedData.Num())
				Cast<AEDBaseNetworkedPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->AddVoiceData(compressedData);
			}
		}
	}
}

void UPlayerVoiceComponent::PlayRemoteVoices() const
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEDPlayerCharacter::StaticClass(), actors);

	// Loop over all available player voices
	for (APlayerVoice* const voice : VoiceManager->GetPlayerVoices())
	{
		// Voice can still be null when it is not yet replicated.
		if (!voice) continue;

		// If voice is from this player continue.
		if (voice->GetPlayerNetId() == PlayerVoiceNetID) continue;

		for (AActor* actor : actors)
		{
			// Check if the voice id of the other actors component == their corresponding PlayerVoice.
			UPlayerVoiceComponent* otherVoiceComp = Cast<UPlayerVoiceComponent>(actor->GetComponentByClass(StaticClass()));
			if (otherVoiceComp->PlayerVoiceNetID == voice->GetPlayerNetId())
			{
				// Play this is voice on the other players voice component.
				otherVoiceComp->PlayVoice(voice->GetVoiceData());
			}
		}
	}
}

void UPlayerVoiceComponent::PlayVoice(const TArray<uint8>& data)
{
	if (SteamUser() && data.Num() > 0)
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
			ED_LOG(LogED_Voice, Fatal, TEXT("UPlayerVoiceComponent::PlayVoice: Decompressed array size too small!"))
		}

		// Set array size to number of valid bytes of voice data
		decompressedData.SetNum(bytesWritten);

		ED_LOG(LogED_Voice, Verbose, TEXT("UPlayerVoiceComponent::PlayVoice: Queuing %i bytes of voice data."), decompressedData.Num())

		// Queue audio for playing
		VoiceSoundWave->QueueAudio(decompressedData.GetData(), decompressedData.Num());
	}
}

void UPlayerVoiceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerVoiceComponent, PlayerVoiceNetID);
}