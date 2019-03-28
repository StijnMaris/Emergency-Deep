

#include "Pump.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/DoubleGripStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Player/GrippableStaticMeshComponent.h"


// Sets default values
APump::APump()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PumpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PumpMesh"));
	Crank = CreateDefaultSubobject<UDoubleGripStaticMeshComponent>(TEXT("Crank"));
	CrankTriggerDown = CreateDefaultSubobject<UBoxComponent>(TEXT("CrankTriggerDown"));
	CrankTriggerUp = CreateDefaultSubobject<UBoxComponent>(TEXT("CrankTriggerUp"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));
	Constraint = CreateDefaultSubobject< UPhysicsConstraintComponent>(TEXT("Constraint"));

	RootComponent = PumpMesh;
	Crank->SetupAttachment(PumpMesh);
	CrankTriggerDown->SetupAttachment(PumpMesh);
	CrankTriggerUp->SetupAttachment(PumpMesh);
	ConstraintPositioner->SetupAttachment(PumpMesh);
	Constraint->SetupAttachment(ConstraintPositioner);

	Crank->SetCollisionProfileName(TEXT("Grippable"));
	CrankTriggerUp->SetCollisionProfileName(TEXT("GrippableTrigger"));
	CrankTriggerDown->SetCollisionProfileName(TEXT("GrippableTrigger"));
	Crank->SetSimulatePhysics(true);
	Crank->SetEnableGravity(false);
	Crank->SetLinearDamping(100.0f);
	Crank->SetAngularDamping(100.0f);

	CrankTriggerUp->OnComponentBeginOverlap.AddDynamic(this, &APump::OnBeginOverlap);
	CrankTriggerDown->OnComponentBeginOverlap.AddDynamic(this, &APump::OnBeginOverlap);

	CrankTriggerUp->OnComponentEndOverlap.AddDynamic(this, &APump::OnEndOverlap);
	CrankTriggerDown->OnComponentEndOverlap.AddDynamic(this, &APump::OnEndOverlap);

	Constraint->ComponentName1 = FConstrainComponentPropName{ FName{ "PumpMesh" } };
	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{ "Crank" } };
}

// Called when the game starts or when spawned
void APump::BeginPlay()
{
	Super::BeginPlay();
	
	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame1, ConstraintPositioner->RelativeLocation);
	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame2, FVector{ 0.0f });
	
	Constraint->SetConstraintReferenceOrientation(EConstraintFrame::Frame2, FVector{ 1.0f, 0.0f, 0.0f }, FVector{ 0.0f, 1.0f, 0.0f });
	
	Constraint->SetLinearXLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearYLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearZLimit(LCM_Locked, 0.0f);
	Constraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	Constraint->SetAngularSwing2Limit(ACM_Limited, CrankMaxAngle);

	CurrentPumpTime = 0;
}

// Called every frame
void APump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePump(DeltaTime);
	Crank->UpdateGrip(DeltaTime);
}

void APump::PumpWater()
{
	if (IsCharged)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pumping"));
		CurrentPumpTime = PumpingTime;
		IsPumping = true;
		IsCharged = false;
	}	
}

void APump::ChargePump()
{
	UE_LOG(LogTemp, Warning, TEXT("Charging"));
	IsCharged = true;
}

void APump::OnBeginOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherComp == Crank)
	{
		if (overlappedComp == CrankTriggerUp)
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerUP"));
			ChargePump();
		}
		else if (overlappedComp == CrankTriggerDown)
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerDOWN"));
			PumpWater();
		}
	}
}

void APump::OnEndOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex)
{
}

void APump::UpdatePump(float deltaTime)
{
	CurrentPumpTime -= deltaTime;
	if (CurrentPumpTime < 0)
	{
		CurrentPumpTime = 0; 
		IsPumping = false;
	}

}

void APump::OnGrab_Implementation(USceneComponent * MotionController, UPhysicsHandleComponent * PhysicsHandle, FVector GrabLocation)
{
	Crank->OnGrab(MotionController, PhysicsHandle, GrabLocation);
	Crank->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
}

void APump::OnGrabReleaseController_Implementation(UPhysicsHandleComponent * PhysicsHandle, USceneComponent* MotionController)
{
	Crank->DettachComponent(PhysicsHandle);
	Crank->OnGrabReleaseController(PhysicsHandle, MotionController);

}