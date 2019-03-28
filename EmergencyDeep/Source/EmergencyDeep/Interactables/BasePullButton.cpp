

#include "BasePullButton.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ABasePullButton::ABasePullButton()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	ButtonBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBaseMesh"));
	ButtonMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("ButtonMesh"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));

	RootComponent = ButtonBaseMesh;
	ButtonMesh->SetupAttachment(RootComponent);
	TriggerBox->SetupAttachment(RootComponent);
	ConstraintPositioner->SetupAttachment(RootComponent);
	Constraint->SetupAttachment(ConstraintPositioner);

	ButtonBaseMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	ButtonMesh->SetSimulatePhysics(true);
	ButtonMesh->SetCollisionProfileName(TEXT("Grippable"));
	TriggerBox->SetCollisionProfileName(TEXT("GrippableTrigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABasePullButton::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABasePullButton::OnEndOverlap);

	Constraint->ComponentName1 = FConstrainComponentPropName{ FName{ "ButtonBaseMesh" } };
	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{ "ButtonMesh" } };

	Toggled = false;
}

void ABasePullButton::BeginPlay()
{
	Super::BeginPlay();

	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame1, ConstraintPositioner->RelativeLocation);
	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame2, FVector{ 0.0f });

	Constraint->SetLinearXLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearZLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearYLimit(LCM_Limited, LinearLimit);
	Constraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	Constraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	
	Constraint->SetLinearVelocityDrive(false, true, false);
	Constraint->SetLinearVelocityTarget(FVector{ 0, MotorForce, 0 });
	Constraint->SetLinearDriveParams(0, 1, MotorForce);
}

void ABasePullButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float currentPullDistance = FVector::Distance(ButtonBaseMesh->GetComponentLocation(), ButtonMesh->GetComponentLocation());

	if (!WasPulled && currentPullDistance > PullDistance)
	{
		WasPulled = true;
		OnPullButtonPulledMovedBegin();
	}
	else if(WasPulled && currentPullDistance < PullDistance)
	{
		WasPulled = false;
		OnPullButtonPulledMovedEnd();
	}

	PullDistance = currentPullDistance;
}

void ABasePullButton::OnPullButtonPulled()
{
	IsPulled = true;
	Toggled = !Toggled;

	OnPullButtonPulled_BP();
}

void ABasePullButton::OnPullButtonReleased()
{
	IsPulled = false;

	OnPullButtonReleased_BP();
}

void ABasePullButton::OnPullButtonPulledMovedBegin()
{
	OnPullButtonPulledMovedBegin_BP();
}

void ABasePullButton::OnPullButtonPulledMovedEnd()
{
	OnPullButtonPulledMovedEnd_BP();
}

void ABasePullButton::SetToggle(bool toggled)
{
	Toggled = toggled;
}

void ABasePullButton::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	ButtonMesh->DettachComponent(PhysicsHandle);
}

void ABasePullButton::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	ButtonMesh->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
}

void ABasePullButton::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(otherComp == ButtonMesh)
	{
		OnPullButtonPulled();
	}
}

void ABasePullButton::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherComp == ButtonMesh)
	{
		OnPullButtonReleased();
	}
}

