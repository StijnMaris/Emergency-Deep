

#include "PlayerVoice.h"
#include "Interactables/Communication/Phone.h"
#include "Interactables/Communication/Speaker.h"

APlayerVoice::APlayerVoice()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
}

void APlayerVoice::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
}

void APlayerVoice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ResetVoiceData();
}

void APlayerVoice::SetPlayerNetId(int32 playerNetId)
{
	PlayerNetId = playerNetId;
}

int32 APlayerVoice::GetPlayerNetId() const
{
	return PlayerNetId;
}

void APlayerVoice::AddPlayerVoiceData(const TArray<uint8>& data)
{
	PlayerVoiceData.Append(data);

	// Send all this data to every client in the game.
	Server_QueueRemoteVoiceData(data);
}

void APlayerVoice::Server_QueueRemoteVoiceData_Implementation(const TArray<uint8>& data)
{
	if(!HasAuthority())
	{
		PlayerVoiceData.Append(data);
	}
}

void APlayerVoice::ResetVoiceData()
{
	PlayerVoiceData.Empty();
}

const TArray<uint8>& APlayerVoice::GetVoiceData() const
{
	return PlayerVoiceData;
}

void APlayerVoice::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerVoice, PlayerNetId);
}

