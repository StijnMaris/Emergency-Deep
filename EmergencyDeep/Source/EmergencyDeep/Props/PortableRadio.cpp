

#include "PortableRadio.h"
#include "Components/AudioComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "Interactables/BaseButton.h"
#include "Game/PlayerControllers/EDBaseNetworkedPlayerController.h"

APortableRadio::APortableRadio()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("RadioMesh"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	PlayButton = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayButton"));

	RootComponent = MeshComponent;
	AudioComponent->SetupAttachment(RootComponent);
	PlayButton->SetupAttachment(RootComponent);

	MeshComponent->SetSimulatePhysics(true);

	AudioComponent->OnAudioFinished.AddDynamic(this, &APortableRadio::OnSongFinished);

	CurrentSongIndex = 0;
	IsPlaying = false;
}

void APortableRadio::BeginPlay()
{
	Super::BeginPlay();

	Cast<ABaseButton>(PlayButton->GetChildActor())->OnButtonPressedDelegate.AddDynamic(this, &APortableRadio::OnPlayButtonPressed);
}

void APortableRadio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortableRadio::Play()
{
	check(HasAuthority())

	NetMulticast_Play(CurrentSongIndex);
}

void APortableRadio::Stop()
{
	check(HasAuthority())

	NetMulticast_Stop();
}

void APortableRadio::PlaySong(const int32 currentSong)
{
	check(Songs.IsValidIndex(currentSong))
	ED_LOG(LogED_Game, Verbose, TEXT("APortableRadio::PlaySong"))

	CurrentSongIndex = currentSong;
	IsPlaying = true;
	SetCurrentSound();
	AudioComponent->Play();
}

void APortableRadio::NetMulticast_Play_Implementation(const int32 currentSong)
{
	PlaySong(currentSong);
}

void APortableRadio::Next()
{
	if(!Songs.IsValidIndex(++CurrentSongIndex))
	{
		CurrentSongIndex = 0;
	}

	SetCurrentSound();
}

void APortableRadio::Previous()
{
	if (!Songs.IsValidIndex(--CurrentSongIndex))
	{
		CurrentSongIndex = Songs.Num() - 1;
	}

	SetCurrentSound();
}

void APortableRadio::NetMulticast_Stop_Implementation()
{
	ED_LOG(LogED_Game, Verbose, TEXT("APortableRadio::NetMulticast_Stop_Implementation"))
	IsPlaying = false;
	AudioComponent->Stop();
}

void APortableRadio::OnSongFinished()
{
	if (IsPlaying) AudioComponent->Stop();

	Next();

	if (IsPlaying) AudioComponent->Play();
}

void APortableRadio::SetCurrentSound()
{
	AudioComponent->SetSound(Songs[CurrentSongIndex]);
}

void APortableRadio::OnPlayButtonPressed()
{
	if(IsPlaying)
	{
		Cast<AEDBaseNetworkedPlayerController>(GetWorld()->GetFirstPlayerController())->StopPortableRadio(this);
	}
	else
	{
		Cast<AEDBaseNetworkedPlayerController>(GetWorld()->GetFirstPlayerController())->PlayPortableRadio(this);
	}
}

void APortableRadio::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortableRadio, CurrentSongIndex);
	DOREPLIFETIME(APortableRadio, IsPlaying);
}

void APortableRadio::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	MeshComponent->AttachActor(MotionController, RootComponent, MeshComponent);
}

void APortableRadio::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	MeshComponent->DettachActor(RootComponent, MeshComponent, true);
}