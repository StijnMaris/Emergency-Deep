

#include "PhoneHorn.h"
#include "Online/Voice/VoiceRecorderComponent.h"
#include "Online/Voice/VoiceSpeakerComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "Online/Voice/PlayerVoice.h"
#include "Sound/SoundAttenuation.h"
#include "Phone.h"


APhoneHorn::APhoneHorn()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	RecorderComponent = CreateDefaultSubobject<UVoiceRecorderComponent>(TEXT("RecorderComponent"));
	SpeakerPositionerComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpeakerPositionerComponent"));
	PhoneHornMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("PhoneHornMesh"));
	PhoneHornCablePosition = CreateDefaultSubobject<USceneComponent>(TEXT("PhoneHornCablePosition"));

	RootComponent = PhoneHornMesh;
	RecorderComponent->SetupAttachment(PhoneHornMesh);
	SpeakerPositionerComponent->SetupAttachment(PhoneHornMesh);
	PhoneHornCablePosition->SetupAttachment(PhoneHornMesh);

	PhoneHornMesh->SetSimulatePhysics(true);
	PhoneHornMesh->SetLinearDamping(1.0f);
	PhoneHornMesh->SetAngularDamping(1.0f);

	Volume = 1.0f;
	MaxRecordingDistance = 100.0f;

}

void APhoneHorn::BeginPlay()
{
	Super::BeginPlay();

	RecorderComponent->SetIsRecording(true);
	RecorderComponent->MaxRecordingDistance = MaxRecordingDistance;
}

void APhoneHorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateComponentCount();
	PlayVoices();
}

void APhoneHorn::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{	
	PhoneHornMesh->AttachActor(MotionController, RootComponent, PhoneHornMesh);
}

void APhoneHorn::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	PhoneHornMesh->DettachActor(RootComponent, PhoneHornMesh,true);
}

void APhoneHorn::UpdateComponentCount()
{
	const int32 numberOfPlayers = GetWorld()->GetGameState()->PlayerArray.Num();

	if (numberOfPlayers != SpeakerComponents.Num())
	{
		for (UVoiceSpeakerComponent* speakerComponent : SpeakerComponents)
		{
			speakerComponent->DestroyComponent();
		}
		SpeakerComponents.Empty();

		TArray<APlayerVoice*> playerVoices = Helpers::GetAllActorsOfType<APlayerVoice>(GetWorld());
		if (numberOfPlayers == playerVoices.Num())
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

void APhoneHorn::PlayVoices()
{
	TArray<APhoneHorn*> phones = Helpers::GetAllActorsOfType<APhoneHorn>(GetWorld());
	for (APhoneHorn* phone : phones)
	{
		if (phone == this) continue;

		for (const FPlayerRecordingData& recordedData : RecorderComponent->GetRecordedData())
		{
			for (UVoiceSpeakerComponent* speakerComponent : phone->SpeakerComponents)
			{
				if (speakerComponent->PlayerNetId == recordedData.PlayerId)
				{
					speakerComponent->PlayVoiceData(recordedData.Voice->GetVoiceData(), recordedData.VolumeMultiplier);
				}
			}
		}
	}
}