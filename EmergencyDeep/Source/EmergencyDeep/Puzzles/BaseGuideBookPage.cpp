#include "BaseGuideBookPage.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ABaseGuideBookPage::ABaseGuideBookPage()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PageMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("PageMesh"));
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));

	PageMesh->SetupAttachment(RootComponent);
	ConstraintPositioner->SetupAttachment(RootComponent);
	Constraint->SetupAttachment(ConstraintPositioner);

	PageMesh->SetCollisionProfileName(TEXT("Grippable"));
	PageMesh->SetSimulatePhysics(true);
	PageMesh->SetEnableGravity(false);
	PageMesh->SetLinearDamping(1000.0f);
	PageMesh->SetAngularDamping(1000.0f);

	Constraint->ComponentName1 = FConstrainComponentPropName{ FName{ "BookMesh" } };
	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{ "PageMesh" } };
}

void ABaseGuideBookPage::BeginPlay()
{
	Super::BeginPlay();

	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame1, ConstraintPositioner->RelativeLocation);
	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame2, FVector{ 0.0f });

	Constraint->SetLinearXLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearYLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearZLimit(LCM_Locked, 0.0f);
	Constraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	Constraint->SetAngularSwing2Limit(ACM_Limited, 180.0f);
}

void ABaseGuideBookPage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGuideBookPage::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	PageMesh->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
}

void ABaseGuideBookPage::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	PageMesh->DettachComponent(PhysicsHandle);
}
