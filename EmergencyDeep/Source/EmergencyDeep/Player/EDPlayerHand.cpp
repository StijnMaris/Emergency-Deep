#include "EDPlayerHand.h"
#include "EDPlayerCharacter.h"
#include "TeleportationComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EDGrabInterface.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "GrippableStaticMeshComponent.h"
#include "DoubleGripStaticMeshComponent.h"
#include "Player/CharacterHandAnimInstance.h"

AEDPlayerHand::AEDPlayerHand()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	HandMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMeshComponent"));
	ArcDirectionComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArcDirectionComponent"));
	ArcSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("ArcSplineComponent"));
	GrabSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereComponent"));
	ArcEndpointComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcEndpointComponent"));
	TeleportLocationMarkerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportLocationMarkerComponent"));
	TeleportationComponent = CreateDefaultSubobject<UTeleportationComponent>(TEXT("TeleportationComponent"));
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));

	SetRootComponent(SceneRootComponent);
	HandMeshComponent->SetupAttachment(SceneRootComponent);
	ArcDirectionComponent->SetupAttachment(HandMeshComponent);
	ArcSplineComponent->SetupAttachment(HandMeshComponent);
	GrabSphereComponent->SetupAttachment(HandMeshComponent);
	ArcEndpointComponent->SetupAttachment(SceneRootComponent);
	TeleportLocationMarkerComponent->SetupAttachment(RootComponent);
	TeleportationComponent->SetupAttachment(RootComponent);

	HandMeshComponent->SetCollisionProfileName(TEXT("PlayerHands"));
	HandMeshComponent->SetGenerateOverlapEvents(true);
	//HandMeshComponent->SetOnlyOwnerSee(true);

	GrabSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GrabSphereComponent->SetCollisionObjectType(ECC_GameTraceChannel4);

	Hand = EControllerHand::Right;

	PhysicsHandleComponent->bSoftLinearConstraint = false;
	PhysicsHandleComponent->bSoftAngularConstraint = false;
	PhysicsHandleComponent->SetIsReplicated(true);
}

void AEDPlayerHand::Initialize(EControllerHand hand)
{
	Hand = hand;
}

void AEDPlayerHand::BeginPlay()
{
	Super::BeginPlay();

	SetupCollision();
	SetupTeleportation();
	OriginalHandLocation = HandMeshComponent->RelativeLocation;
}

void AEDPlayerHand::SetupCollision() const
{
	TeleportLocationMarkerComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ArcEndpointComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GrabSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AEDPlayerHand::SetupTeleportation() const
{
	TeleportationComponent->SetupComponent(ArcDirectionComponent, ArcSplineComponent, TeleportLocationMarkerComponent, Cast<AEDPlayerCharacter>(GetOwner()));
	TeleportLocationMarkerComponent->SetVisibility(false);
}

void AEDPlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTeleportation();
	UpdateGrab();
	UpdateHandAnimation();
}

void AEDPlayerHand::UpdateTeleportation() const
{
	if (Hand == EControllerHand::Right && IsTeleportPressed)
	{
		TeleportationComponent->UpdateDestinationMarker();
	}
}

void AEDPlayerHand::UpdateGrab()
{
	if (PhysicsHandleComponent)
	{
		GrabLocation = GrabSphereComponent->GetComponentLocation();
		PhysicsHandleComponent->SetTargetLocation(GrabLocation);
	}
}

void AEDPlayerHand::UpdateHandAnimation()
{
	if (WantsToGrip || AttachedActor != nullptr)
	{
		GrabState = EGrab::Grab;
	}
	else if (GetObjectNearHand() != nullptr)
	{
		GrabState = EGrab::CanGrab;
	}
	else
	{
		GrabState = EGrab::Open;
	}

	UCharacterHandAnimInstance* animInstance = Cast<UCharacterHandAnimInstance>(HandMeshComponent->GetAnimInstance());
	check(animInstance)
	animInstance->SetHandState(GrabState);
}

UObject* AEDPlayerHand::GetObjectNearHand()
{
	float nearestOverlap{ 10000.0 };
	UObject* nearestOverlappingObject{};
	TArray<UPrimitiveComponent*> overlappingComponents;

	GrabSphereComponent->GetOverlappingComponents(overlappingComponents);
	FVector vectorToActor{};

	for (UPrimitiveComponent* element : overlappingComponents)
	{
		vectorToActor = element->GetComponentLocation() - GrabSphereComponent->GetComponentLocation();
		float distanceToActor = vectorToActor.Size();

		bool isImplementedOnActor = element->GetOwner()->GetClass()->ImplementsInterface(UEDGrabInterface::StaticClass());
		bool isImplementedOnComponent = element->Implements<UEDGrabInterface>();
		bool isGrippable = element->GetClass() == UGrippableStaticMeshComponent::StaticClass();
		bool IsDoubleGrippable = element->GetClass() == UDoubleGripStaticMeshComponent::StaticClass();
		if ((isGrippable || IsDoubleGrippable)&& isImplementedOnComponent && distanceToActor < nearestOverlap)
		{
			IsActorGrip = false;
			nearestOverlappingObject = element;
			GrabLocation = GrabSphereComponent->GetComponentLocation();
			nearestOverlap = distanceToActor;
		}
		else if ((isGrippable || IsDoubleGrippable) && isImplementedOnActor && distanceToActor < nearestOverlap)
		{
			IsActorGrip = true;
			nearestOverlappingObject = element->GetOwner();
			nearestOverlap = distanceToActor;
		}
		
	}

	return nearestOverlappingObject;
}

void AEDPlayerHand::GrabObject()
{
	Server_GrabObject();
}

void AEDPlayerHand::Server_GrabObject_Implementation()
{
	NetMulticast_GrabObject();
}

ED_DEFAULT_SERVER_VALIDATION(AEDPlayerHand, GrabObject)

void AEDPlayerHand::NetMulticast_GrabObject_Implementation()
{
	WantsToGrip = true;
	if (GetObjectNearHand())
	{
		if (IsActorGrip)
		{
			AttachedActor = Cast<AActor>(GetObjectNearHand());
		}
		else
		{
			AttachedComponent = Cast<UPrimitiveComponent>(GetObjectNearHand());
			AttachedActor = AttachedComponent->GetOwner();
		}
		IEDGrabInterface::Execute_OnGrab(AttachedActor, SceneRootComponent, PhysicsHandleComponent, GrabLocation);
	}
}

void AEDPlayerHand::SideGripObject()
{
	Server_SideGripObject();
}

void AEDPlayerHand::Server_SideGripObject_Implementation()
{
	NetMulticast_SideGripObject();
}

ED_DEFAULT_SERVER_VALIDATION(AEDPlayerHand, SideGripObject)

void AEDPlayerHand::NetMulticast_SideGripObject_Implementation()
{
	if(AttachedComponent || AttachedActor)
	{
		DropObject_Implementation();
	}
	else
	{	
		if (GetObjectNearHand())
		{
			WantsToGrip = true;
			if (IsActorGrip)
			{
				AttachedActor = Cast<AActor>(GetObjectNearHand());
			}
			else
			{
				AttachedComponent = Cast<UPrimitiveComponent>(GetObjectNearHand());
				AttachedActor = AttachedComponent->GetOwner();
			}
			IEDGrabInterface::Execute_OnSideGrip(AttachedActor, SceneRootComponent, PhysicsHandleComponent, GrabLocation);
		}
	}
}

void AEDPlayerHand::DropObject()
{
	Server_DropObject();
}

void AEDPlayerHand::Server_DropObject_Implementation()
{
	NetMulticast_DropObject();
}

ED_DEFAULT_SERVER_VALIDATION(AEDPlayerHand, DropObject)

void AEDPlayerHand::DropObject_Implementation()
{
	WantsToGrip = false;
	if (AttachedActor && IsActorGrip)
	{
		if (SceneRootComponent == AttachedActor->GetRootComponent()->GetAttachParent())
		{
			IEDGrabInterface::Execute_OnGrabRelease(AttachedActor, PhysicsHandleComponent);
			IEDGrabInterface::Execute_OnGrabReleaseController(AttachedActor, PhysicsHandleComponent, SceneRootComponent);
		}
	}
	else if (AttachedComponent)
	{
		IEDGrabInterface::Execute_OnGrabRelease(AttachedActor, PhysicsHandleComponent);
		IEDGrabInterface::Execute_OnGrabReleaseController(AttachedActor, PhysicsHandleComponent, SceneRootComponent);

	}
	AttachedActor = nullptr;
	AttachedComponent = nullptr;
	IsActorGrip = false;
}

void AEDPlayerHand::NetMulticast_DropObject_Implementation()
{
	DropObject_Implementation();
}

void AEDPlayerHand::ActivateTeleport(bool large)
{
	IsTeleportPressed = true;
}

void AEDPlayerHand::DeActivateTeleport()
{
	IsTeleportPressed = false;
	if (TeleportationComponent->IsTeleporting() == false && TeleportationComponent->CanTeleport())
	{
		TeleportationComponent->BeginTeleport();
	}
	else
	{
		TeleportationComponent->ResetDestination();
	}
}

EGrab AEDPlayerHand::GetGrabState() const
{
	return GrabState;
}

FVector AEDPlayerHand::GetForward() const
{
	return SceneRootComponent->GetForwardVector();
}

FVector AEDPlayerHand::GetRight() const
{
	return SceneRootComponent->GetRightVector();
}

AActor* AEDPlayerHand::GetAttachedActor() const
{
	return AttachedActor;
}

void AEDPlayerHand::ResetHandMesh()
{
	HandMeshComponent->SetRelativeLocation(OriginalHandLocation);
}

void AEDPlayerHand::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEDPlayerHand, Hand);
	DOREPLIFETIME(AEDPlayerHand, GrabState);
}