

#include "SpeakerRecorder.h"
#include "Online/Voice/VoiceRecorderComponent.h"
#include "Interactables/BaseButton.h"
#include "Online/Voice/PlayerVoice.h"
#include "Sound/SoundWaveProcedural.h"
#include "Speaker.h"
#include "Game/PlayerControllers/EDBaseNetworkedPlayerController.h"

ASpeakerRecorder::ASpeakerRecorder()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RecorderComponent = CreateDefaultSubobject<UVoiceRecorderComponent>(TEXT("Recorder"));
	PushToTalkButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("PushToTalkButton"));

	RootComponent = MeshComponent;
	RecorderComponent->SetupAttachment(RootComponent);
	PushToTalkButton->SetupAttachment(RootComponent);

	MaxRecordingDistance = 100.0f;
}

void ASpeakerRecorder::BeginPlay()
{
	Super::BeginPlay();

	RecorderComponent->MaxRecordingDistance = MaxRecordingDistance;

	Speakers = Helpers::GetAllActorsOfType<ASpeaker>(GetWorld());

	ABaseButton* pushToTalkButton = Cast<ABaseButton>(PushToTalkButton->GetChildActor());
	pushToTalkButton->OnButtonPressedDelegate.AddDynamic(this, &ASpeakerRecorder::OnButtonPressed);
	pushToTalkButton->OnButtonReleasedDelegate.AddDynamic(this, &ASpeakerRecorder::OnButtonReleased);
}

void ASpeakerRecorder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Set and play the recorded data on all connected speakers.
	for (ASpeaker* speaker : Speakers)
	{
		speaker->PlayVoiceData(RecorderComponent->GetRecordedData());
	}
}

void ASpeakerRecorder::ButtonPressed()
{
	RecorderComponent->SetIsRecording(true);
}

void ASpeakerRecorder::ButtonReleased()
{
	RecorderComponent->SetIsRecording(false);
}

void ASpeakerRecorder::OnButtonPressed()
{
	Cast<AEDBaseNetworkedPlayerController>(GetWorld()->GetFirstPlayerController())->StartSpeakerRecorder(this);
}

void ASpeakerRecorder::OnButtonReleased()
{
	Cast<AEDBaseNetworkedPlayerController>(GetWorld()->GetFirstPlayerController())->StopSpeakerRecorder(this);
}
