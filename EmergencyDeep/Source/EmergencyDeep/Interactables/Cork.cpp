#include "Cork.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Tasks/Objects/HullBreach.h"
#include "MotionControllerComponent.h"
#include "Tasks/Objects/Hammer.h"
#include "Player/EDPlayerHand.h"
// Sets default values
ACork::ACork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CorkMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("CorkMesh"));
	HammerTriggerEnter = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerTriggerEnter"));
	HammerTriggerLeave = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerTriggerLeave"));

	RootComponent = CorkMesh;
	HammerTriggerEnter->SetupAttachment(CorkMesh);
	HammerTriggerLeave->SetupAttachment(CorkMesh);

	CorkMesh->SetSimulatePhysics(true);
	HammerTriggerEnter->SetCollisionProfileName("OverlapAll");
	HammerTriggerLeave->SetCollisionProfileName("OverlapAll");
	HammerTriggerLeave->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HammerTriggerEnter->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	HammerTriggerEnter->OnComponentBeginOverlap.AddDynamic(this, &ACork::OnBeginOverlap);
	HammerTriggerLeave->OnComponentEndOverlap.AddDynamic(this, &ACork::OnEndOverlap);

	CurrentUnCorkTime = 0;
	
}

// Called when the game starts or when spawned
void ACork::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACork::OnBeginOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{

	AHammer* hammer = Cast<AHammer>(otherActor);
	if (hammer != nullptr && Plugged && HasHammered == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("HammerTime"));
		HasHammered = true;
		GetHammered();
	}
}

void ACork::OnEndOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex)
{
	AHammer* hammer = Cast<AHammer>(otherActor);
	if (hammer != nullptr && Plugged)
	{
		UE_LOG(LogTemp, Warning, TEXT("HammerLeave"));
		HasHammered = false;
	}
}

void ACork::UpdateUncorking(float deltaTime)
{
	CurrentUnCorkTime -= deltaTime;
	if (CurrentUnCorkTime <= 0)
	{
		Hammered--;
		if (Hammered <= 0)
		{
			//UnCork leak
			CreateLeak();
			ClosedLeak->Damage();
			ClosedLeak = nullptr;
		}
		else
		{
			//Move Cork
			float newOffset = StartOffset - HammerSteps * Hammered;
			SetActorLocation(ClosedLeak->GetActorLocation() + ClosedLeak->GetActorUpVector()*newOffset);

			//SetNextUncorkTime
			ChosenUnCorkTime = ((rand() % int((MaxUnCorkTime - MinUnCorkTime) * 100)) / 100.0f) + MinUnCorkTime;
			CurrentUnCorkTime = ChosenUnCorkTime;
			OnPushed_BP();
		}
	}
}


// Called every frame
void ACork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Plugged)
	{
		UpdateUncorking(DeltaTime);
	}
}

void ACork::GetHammered()
{
	Hammered++;
	if (Hammered > MaxHammered)
	{
		Hammered = MaxHammered;
	}
	// set Position
	float newOffset = StartOffset - HammerSteps * Hammered;
	SetActorLocation(ClosedLeak->GetActorLocation() + ClosedLeak->GetActorUpVector()*newOffset);
	
	// Set UnCorkTime
	ChosenUnCorkTime = ((rand() % int((MaxUnCorkTime - MinUnCorkTime) * 100)) / 100.0f) + MinUnCorkTime;
	CurrentUnCorkTime = ChosenUnCorkTime;
	OnHammered_BP();
}

void ACork::CloseLeak(AHullBreach * leak)
{
	ClosedLeak = leak;
	AEDPlayerHand* hand = Cast<AEDPlayerHand>(MotionContr->GetOwner());
	if (hand)
	{
		hand->DropObject();
	}
	CorkMesh->SetSimulatePhysics(false);
	Plugged = true;
	Hammered = 0;
	SetActorLocation(leak->GetActorLocation() + leak->GetActorUpVector()*StartOffset);
	SetActorRotation(leak->GetActorRotation());
	GetHammered(); //Hammer it once
	OnCorked_BP();

}

void ACork::CreateLeak()
{
	CorkMesh->SetSimulatePhysics(true);
	Plugged = false;
	CorkMesh->AddImpulse(UnCorkForce*ClosedLeak->GetActorUpVector());
	Hammered = 0;
	OnUnCorked_BP();
}

void ACork::OnGrab_Implementation(USceneComponent * motionController, UPhysicsHandleComponent * PhysicsHandle, FVector GrabLocation)
{
	if (Plugged == false)
	{
		CorkMesh->AttachActor(motionController, GetRootComponent(), CorkMesh);
		MotionContr = motionController;
	}
}

void ACork::OnGrabRelease_Implementation(UPhysicsHandleComponent * PhysicsHandle)
{
	if (Plugged == false)
	{
		CorkMesh->DettachActor(GetRootComponent(), CorkMesh, true);
		//MotionContr = nullptr;
	}
}
