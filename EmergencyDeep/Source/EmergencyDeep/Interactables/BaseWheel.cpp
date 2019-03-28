#include "BaseWheel.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ABaseWheel::ABaseWheel() : bLimitValveRotation(false), ValveMaxAngle(180.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	WheelBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelBase"));
	ValveMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("ValveMesh"));
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));
	OnTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OnTriggerBox"));
	OffTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OffTriggerBox"));
	WheelTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WheelTriggerBox"));

	WheelTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWheel::OnBeginOverlap);
	WheelTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABaseWheel::OnEndOverlap);

	RootComponent = WheelBase;
	ValveMesh->SetupAttachment(WheelBase);
	WheelTriggerBox->SetupAttachment(ValveMesh);
	OnTriggerBox->SetupAttachment(WheelBase);
	OffTriggerBox->SetupAttachment(WheelBase);
	ConstraintPositioner->SetupAttachment(RootComponent);
	Constraint->SetupAttachment(ConstraintPositioner);

	WheelBase->SetCollisionProfileName(TEXT("OverlappAll"));
	ValveMesh->SetCollisionProfileName(TEXT("Interactable"));
	ValveMesh->SetSimulatePhysics(true);
	ValveMesh->SetEnableGravity(false);
	ValveMesh->SetLinearDamping(1000.0f);
	ValveMesh->SetAngularDamping(1000.0f);

	Constraint->ComponentName1 = FConstrainComponentPropName{ FName{ "WheelBase" } };
	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{ "ValveMesh" } };
}

void ABaseWheel::BeginPlay()
{
	Super::BeginPlay();

	Constraint->SetLinearXLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearYLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearZLimit(LCM_Locked, 0.0f);

	switch (RotationAxis)
	{
	case ERotationAxis::RA_X:
		Constraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
		Constraint->SetAngularTwistLimit(bLimitValveRotation ? ACM_Limited : ACM_Free, bLimitValveRotation ? ValveMaxAngle : 0.0f);
		Constraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);

		LastFrameRotation = FRotator(ValveMesh->GetRelativeTransform().GetRotation()).Euler().X;
		break;

	case ERotationAxis::RA_Y:
		Constraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
		Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
		Constraint->SetAngularSwing2Limit(bLimitValveRotation ? ACM_Limited : ACM_Free, bLimitValveRotation ? ValveMaxAngle : 0.0f);

		LastFrameRotation = FRotator(ValveMesh->GetRelativeTransform().GetRotation()).Euler().Y;
		break;

	case ERotationAxis::RA_Z:
		Constraint->SetAngularSwing1Limit(bLimitValveRotation ? ACM_Limited : ACM_Free, bLimitValveRotation ? ValveMaxAngle : 0.0f);
		Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
		Constraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);

		LastFrameRotation = FRotator(ValveMesh->GetRelativeTransform().GetRotation()).Euler().Z;
		break;
	}
}

void ABaseWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float newRotation = 0.0f;
	switch (RotationAxis)
	{
	case ERotationAxis::RA_X:
	{
		auto rot = FRotator(ValveMesh->GetRelativeTransform().GetRotation()).Euler();
		newRotation = rot.X;
		break;
	}
	case ERotationAxis::RA_Y:
	{
		auto rot = FRotator(ValveMesh->GetRelativeTransform().GetRotation()).Euler();
		newRotation = rot.Y;
		break;
	}
	case ERotationAxis::RA_Z:
	{
		auto rot = FRotator(ValveMesh->GetRelativeTransform().GetRotation()).Euler();
		newRotation = rot.Z;
		break;
	}
	}

	if (LastFrameRotation != newRotation)
	{
		float deltaRotation = LastFrameRotation - newRotation;
		if (deltaRotation >= 5.0f || deltaRotation <= -5.0f) deltaRotation = 0.0f;
		OnValveRotated_BP(deltaRotation, newRotation);
	}

	LastFrameRotation = newRotation;
}

bool ABaseWheel::IsValveOn()
{
	return bOnOff;
}

void ABaseWheel::OnBeginOverlap(UPrimitiveComponent *overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherActor == this)
	{
		if(otherComp == OnTriggerBox)
		{
			bOnOff = true;
			OnValveRotatedToggle_BP(bOnOff);
		}
		else if (otherComp == OffTriggerBox)
		{
			bOnOff = false;
			OnValveRotatedToggle_BP(bOnOff);
		}
	}
}

void ABaseWheel::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
}

void ABaseWheel::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	ValveMesh->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
}

void ABaseWheel::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	ValveMesh->DettachComponent(PhysicsHandle);
}
