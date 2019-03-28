#include "NewPipe.h"
#include "Components/BoxComponent.h"
#include "../../Player/GrippableStaticMeshComponent.h"
#include "UnrealNetwork.h"
#include "MotionControllerComponent.h"

ANewPipe::ANewPipe()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
	Pipe = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Pipe"));
	/*TriggerBoxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxLeft"));
	TriggerBoxRight = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxRight"));*/

	RootComponent = Pipe;

	Pipe->SetSimulatePhysics(true);

	Pipe->SetIsReplicated(true);
	/*TriggerBoxLeft->SetupAttachment(Pipe);
	TriggerBoxRight->SetupAttachment(Pipe);*/

	Pipe->SetSimulatePhysics(true);
}

void ANewPipe::BeginPlay()
{
	Super::BeginPlay();
}

void ANewPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewPipe::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	Pipe->AttachActor(MotionController, RootComponent, Pipe);
	if (MotionController)
	{
		HandRight = MotionController;
	}
}

void ANewPipe::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	Pipe->DettachActor(RootComponent, Pipe, true);
}

void ANewPipe::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANewPipe, Pipe);
	/*DOREPLIFETIME(ANewPipe, TriggerBoxRight);
	DOREPLIFETIME(ANewPipe, TriggerBoxLeft);*/
}