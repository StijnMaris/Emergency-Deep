#include "Mop.h"
#include "../../Player/GrippableStaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UnrealNetwork.h"


AMop::AMop()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
	Mop = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Mop"));
	CleaningPos = CreateDefaultSubobject<USceneComponent>(TEXT("CleaningPos"));
	Flames = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flames"));

	Mop->SetupAttachment(RootComponent);
	CleaningPos->SetupAttachment(Mop);
	Flames->SetupAttachment(CleaningPos);

	Mop->SetSimulatePhysics(true);

	Mop->SetIsReplicated(true);
	CleaningPos->SetIsReplicated(true);
	Flames->SetIsReplicated(true);


	IsOnFire = false;
	TimeOnFire = 0;
}

void AMop::BeginPlay()
{
	Super::BeginPlay();
}

void AMop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOnFire)
	{
		Flames->Activate();
		Flames->SetWorldRotation(FRotator{0});
		TimeOnFire += DeltaTime;
		if (TimeOnFire > 20.f)
		{
			Destroy();
		}
	}
	else
	{
		Flames->Deactivate();
	}
}

void AMop::CatchFire()
{
	IsOnFire = true;
	OnCatchFire();
}

void AMop::StopFire()
{
	IsOnFire = false;
	TimeOnFire = 0;
	OnStopFire();
}

//bool AMop::Server_StopFire_Validate()
//{
//	return true;
//}
//
//void AMop::Server_StopFire_Implementation()
//{
//	
//}

FVector AMop::GetCleaningPos()
{
	return CleaningPos->GetComponentLocation();
}

void AMop::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	Mop->AttachActor(MotionController, RootComponent, Mop);
}

void AMop::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	Mop->DettachActor(RootComponent, Mop, true);
}

void AMop::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMop, Mop);
	DOREPLIFETIME(AMop, CleaningPos);
	DOREPLIFETIME(AMop, IsOnFire);
	DOREPLIFETIME(AMop, TimeOnFire);
}