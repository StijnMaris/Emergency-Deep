#include "Hammer.h"
#include "UnrealNetwork.h"

// Sets default values
AHammer::AHammer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hammer = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Hammer"));
	Hammer->SetupAttachment(RootComponent);

	Hammer->SetSimulatePhysics(true);

	Hammer->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AHammer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHammer::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	Hammer->AttachActor(MotionController, RootComponent, Hammer);
}

void AHammer::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	Hammer->DettachActor(RootComponent, Hammer, true);
}

void AHammer::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHammer, Hammer);
}