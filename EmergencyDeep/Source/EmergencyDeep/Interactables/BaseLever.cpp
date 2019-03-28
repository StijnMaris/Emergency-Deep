#include "BaseLever.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Player/EDPlayerHand.h"

ABaseLever::ABaseLever()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	HandleMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("HandleMesh"));
	HandleTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HandleTriggerBox"));
	TopTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopTriggerBox"));
	BottomTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomTriggerBox"));
	GrabReleaseCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GrabReleaseCapsuleComponent"));

	RootComponent = LeverMesh;
	HandleMesh->SetupAttachment(LeverMesh);
	HandleTriggerBox->SetupAttachment(HandleMesh);
	TopTriggerBox->SetupAttachment(RootComponent);
	BottomTriggerBox->SetupAttachment(RootComponent);
	GrabReleaseCapsuleComponent->SetupAttachment(HandleMesh);

	LeverMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	HandleMesh->SetCollisionProfileName(TEXT("Grippable"));
	BottomTriggerBox->SetCollisionProfileName(TEXT("GrippableTrigger"));
	TopTriggerBox->SetCollisionProfileName(TEXT("GrippableTrigger"));
	HandleTriggerBox->SetCollisionProfileName(TEXT("GrippableTrigger"));

	HandleTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseLever::OnBeginOverlap);
	HandleTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABaseLever::OnEndOverlap);

	GrabReleaseCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseLever::OnLeaveGrabSphere);

	LeverMaxAngle = 35.0f;
	MinMovementEventTriggerValue = 1.0f;
	BaseOffset = -90.0f;
}

void ABaseLever::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLeverRotation();
}

void ABaseLever::UpdateLeverRotation()
{
	if (!CurrentInteractor) return;

	FTransform leverTransform = GetActorTransform();
	const FVector interactorLocation = CurrentInteractor->GetComponentLocation();

	// Calculate lever angle
	const FVector inverseLocation = leverTransform.InverseTransformPosition(interactorLocation);
	const FVector handleRelativeLocation = HandleMesh->RelativeLocation;
	const FVector difference = inverseLocation - handleRelativeLocation;
	const float angle = FMath::RadiansToDegrees(FMath::Atan2(difference.Z, difference.X));
	const float clampedAngle = BaseOffset + FMath::Clamp(angle, LeverMinAngle, LeverMaxAngle);

	// Calculate lever movement
	const float movement = abs(HandleMesh->RelativeRotation.Pitch - clampedAngle);
	if (!HasBeginMoved && movement >= MinMovementEventTriggerValue)
	{
		OnHandleMoveBegin();
		HasBeginMoved = true;
	}
	else if(HasBeginMoved &&  movement < MinMovementEventTriggerValue)
	{
		OnHandleMoveEnd();
		HasBeginMoved = false;
	}	

	// Set new lever angle
	HandleMesh->SetRelativeRotation(FRotator{ clampedAngle, 0.0f, 0.0f });
}

void ABaseLever::OnTopTriggerBegin()
{
	LeverState = ELeverState::TOP;

	OnTopTriggerBegin_BP();

	OnTopTriggerEnterDelegate.Broadcast();
}

void ABaseLever::OnTopTriggerEnd()
{
	LeverState = ELeverState::CENTER;

	OnTopTriggerEnd_BP();

	OnTopTriggerLeaveDelegate.Broadcast();
}

void ABaseLever::OnBottomTriggerBegin()
{
	LeverState = ELeverState::BOTTOM;

	OnBottomTriggerBegin_BP();

	OnBotTriggerEnterDelegate.Broadcast();
}

void ABaseLever::OnBottomTriggerEnd()
{
	LeverState = ELeverState::CENTER;

	OnBottomTriggerEnd_BP();

	OnBotTriggerLeaveDelegate.Broadcast();
}

void ABaseLever::OnHandleMoveBegin()
{
	OnHandleMoveBegin_BP();

	OnHandleMoveBeginDelegate.Broadcast();
}

void ABaseLever::OnHandleMoveEnd()
{
	OnHandleMoveEnd_BP();

	OnHandleMoveEndDelegate.Broadcast();
}

void ABaseLever::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	CurrentInteractor = MotionController;
}

void ABaseLever::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	CurrentInteractor = nullptr;
}

void ABaseLever::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(otherComp == TopTriggerBox)
	{
		OnTopTriggerBegin();
	}
	else if(otherComp == BottomTriggerBox)
	{
		OnBottomTriggerBegin();
	}
}

void ABaseLever::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherComp == TopTriggerBox)
	{
		OnTopTriggerEnd();
	}
	else if (otherComp == BottomTriggerBox)
	{
		OnBottomTriggerEnd();
	}
}

void ABaseLever::OnLeaveGrabSphere(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	AEDPlayerHand* hand = Cast<AEDPlayerHand>(otherActor);
	if(hand && hand->HandMeshComponent == otherComp)
	{
		CurrentInteractor = nullptr;
	}
}

