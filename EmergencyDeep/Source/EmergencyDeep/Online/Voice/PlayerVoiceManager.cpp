

#include "PlayerVoiceManager.h"
#include "Online/Voice/PlayerVoiceComponent.h"

APlayerVoiceManager::APlayerVoiceManager()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
}

void APlayerVoiceManager::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerVoiceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerVoiceManager::RegisterPlayer(APlayerController* owner, UPlayerVoiceComponent* voiceComponent, int32 playerNetId)
{
	// Check if player is already registered
	for (APlayerVoice* playerVoice : PlayerVoices)
	{
		if (playerVoice->GetPlayerNetId() == playerNetId)
		{
			ED_LOG(LogED_Voice, Warning, TEXT("APlayerVoiceManager::RegisterPlayer: Player %i already registered."), playerNetId);
			return;
		}
	}

	// Create a player voice and set all ids to the owners player id.
	APlayerVoice* newVoice = Cast<APlayerVoice>(GetWorld()->SpawnActor(APlayerVoice::StaticClass()));
	newVoice->SetPlayerNetId(playerNetId);
	newVoice->SetOwner(owner);

	voiceComponent->SetPlayerNetID(playerNetId);

	PlayerVoices.Add(newVoice);

	ED_LOG(LogED_Voice, Log, TEXT("UPlayerVoiceManager::RegisterPlayer: Registered new player %i."), playerNetId);
}

void APlayerVoiceManager::UnRegisterPlayer(int32 playerNetId)
{
	for (APlayerVoice* playerVoice : PlayerVoices)
	{
		if (playerVoice->GetPlayerNetId() == playerNetId)
		{
			PlayerVoices.Remove(playerVoice);
			ED_LOG(LogED_Voice, Log, TEXT("APlayerVoiceManager::UnRegisterPlayer: Player %i unregistered."), playerNetId);
			return;
		}
	}

	ED_LOG(LogED_Voice, Warning, TEXT("UPlayerVoiceManager::UnRegisterPlayer: Player %i was not registered."), playerNetId);
}

const TArray<APlayerVoice*>& APlayerVoiceManager::GetPlayerVoices() const
{
	return PlayerVoices;
}

APlayerVoice* APlayerVoiceManager::GetVoice(int32 id)
{
	for (APlayerVoice* playerVoice : PlayerVoices)
	{
		if (playerVoice)
		{
			if (playerVoice->GetPlayerNetId() == id)
			{
				return playerVoice;
			}
		}
	}

	return nullptr;
}

void APlayerVoiceManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerVoiceManager, PlayerVoices);
}

