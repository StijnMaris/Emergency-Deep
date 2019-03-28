#include "FireExtinguisherTube.h"
#include "../../Player/GrippableStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "OilSpill.h"
#include "UnrealNetwork.h"

AFireExtinguisherTube::AFireExtinguisherTube()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	Tube = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Tube"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Smoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flames"));

	RootComponent = Tube;
	TriggerBox->SetupAttachment(Tube);
	Smoke->SetupAttachment(Tube);

	Tube->SetIsReplicated(true);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFireExtinguisherTube::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFireExtinguisherTube::OnEndOverlap);
}

void AFireExtinguisherTube::BeginPlay()
{
	Super::BeginPlay();
}

void AFireExtinguisherTube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireExtinguisherTube::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	Tube->AttachActor(MotionController, RootComponent, Tube);
	IsGripped = true;
}

void AFireExtinguisherTube::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	Tube->DettachActor(RootComponent, Tube, true);
	IsGripped = false;
}

void AFireExtinguisherTube::Activate()
{
	Smoke->Activate();
	IsActive = true;
}

void AFireExtinguisherTube::Deactivate()
{
	Smoke->Deactivate();
	IsActive = false;
}

void AFireExtinguisherTube::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (IsActive)
	{
		AOilSpill* oilSpill = Cast<AOilSpill>(otherActor);
		if (oilSpill)
		{
			oilSpill->FireGrowState = EGrowState::Shrinking;
		}
		AMop* mop = Cast<AMop>(otherActor);
		if (mop)
		{
			mop->StopFire();
		}
	}
}

void AFireExtinguisherTube::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (IsActive)
	{
		AOilSpill* oilSpill = Cast<AOilSpill>(otherActor);
		if (oilSpill)
		{
			oilSpill->FireGrowState = EGrowState::Growing;
		}
	}
}

void AFireExtinguisherTube::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFireExtinguisherTube, Tube);
    DOREPLIFETIME(AFireExtinguisherTube, IsActive);
	DOREPLIFETIME(AFireExtinguisherTube, IsGripped);
	
}