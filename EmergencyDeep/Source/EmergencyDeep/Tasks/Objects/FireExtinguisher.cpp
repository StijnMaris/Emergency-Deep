#include "FireExtinguisher.h"
#include "../../Player/GrippableStaticMeshComponent.h"
#include "../../Tasks/Objects/Mop.h"
#include "CableComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FireExtinguisherTube.h"
#include "UnrealNetwork.h"

AFireExtinguisher::AFireExtinguisher()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	Base = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Base"));
	//Cable = CreateDefaultSubobject<UCableComponent>(TEXT("cable"));

	Tube = CreateDefaultSubobject<UChildActorComponent>(TEXT("Tube"));

	RootComponent = Base;
	//Cable->SetupAttachment(Base);
	Tube->SetupAttachment(Base);

	Base->SetIsReplicated(true);
	Tube->SetIsReplicated(true);

	//Cable->CableWidth = 5;
	//Cable->CableLength = 10;

	IsGripped = false;
}

void AFireExtinguisher::BeginPlay()
{
	Super::BeginPlay();
}

void AFireExtinguisher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGripped)
	{
		AFireExtinguisherTube* tube = Cast<AFireExtinguisherTube>(Tube->GetChildActor());
		if (tube)
		{
			if (tube->IsGripped)
			{
				tube->Activate();
				OnActivate();
			}
			else
			{
				tube->Deactivate();
				OnDeactivate();
			}
		}
	}
}

void AFireExtinguisher::OnSideGrip_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle,
	FVector GrabLocation)
{
	if (!Base->HasBeenGripped())
	{
		Base->AttachActor(MotionController, RootComponent, Base);
		IsGripped = true;
	}
	else
	{
		Base->DettachActor(RootComponent, Base, true);

		IsGripped = false;
		Cast<AFireExtinguisherTube>(Tube->GetChildActor())->Deactivate();
	}
}

void AFireExtinguisher::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFireExtinguisher, Tube);
    DOREPLIFETIME(AFireExtinguisher, Base);
	DOREPLIFETIME(AFireExtinguisher, IsGripped);
	
}