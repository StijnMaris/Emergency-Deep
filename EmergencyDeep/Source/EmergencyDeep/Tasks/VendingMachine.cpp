#include "VendingMachine.h"
#include "../Interactables/BasePullButton.h"
#include "Engine/World.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "Printables.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AVendingMachine::AVendingMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	VendingMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VendingMachineMesh"));
	VendingMachineMesh->SetupAttachment(RootComp);

	RootButtons = CreateDefaultSubobject<USceneComponent>(TEXT("RootButtons"));
	RootButtons->SetupAttachment(VendingMachineMesh);

	LootCabin = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("LootCabin"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));

	LootCabin->SetupAttachment(VendingMachineMesh);
	TriggerBox->SetupAttachment(RootComp);
	ConstraintPositioner->SetupAttachment(VendingMachineMesh);
	Constraint->SetupAttachment(ConstraintPositioner);

	LootCabin->SetSimulatePhysics(true);
	LootCabin->SetCollisionProfileName(TEXT("Grippable"));
	TriggerBox->SetCollisionProfileName(TEXT("GrippableTrigger"));


	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AVendingMachine::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AVendingMachine::OnEndOverlap);

	Constraint->ComponentName1 = FConstrainComponentPropName{ FName{ "VendingMachineMesh" } };
	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{ "LootCabin" } };
}

// Called when the game starts or when spawned
void AVendingMachine::BeginPlay()
{
	Super::BeginPlay();
	SetConstraint();
	CreateButtons();

}

// Called every frame
void AVendingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVendingMachine::UpdateActiveButtons() 
{
	ActiveButtons.Empty();
	for (int i = 0; i < PullButtons.Num();i++)
	{
		if (PullButtons[i]->Toggled)
		{
			ActiveButtons.Add(i + 1);
		}
	}

}

void AVendingMachine::ResetButtons()
{
	for (int i = 0; i < PullButtons.Num(); i++)
	{
		PullButtons[i]->SetToggle(false);
	}
}

void AVendingMachine::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	LootCabin->DettachComponent(PhysicsHandle);
}

void AVendingMachine::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	LootCabin->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
	
}


void AVendingMachine::OnBeginOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherComp == LootCabin)
	{
		IsPulled = true;

		UpdateActiveButtons();
		ResetButtons();

		// Check for possible prints
		int id = CheckPrints();
		if (id >= 0)
		{
			// Valid Print
			UWorld* world = GetWorld();
			FActorSpawnParameters spawnParams{};
			FRotator rotation = LootCabin->GetComponentRotation();
			FVector position = LootCabin->GetComponentLocation();
			position += SpawnPrintOffset;
			APrintables* actor = world->SpawnActor<APrintables>(PrintableSpawn, position, rotation, spawnParams);
			APrintables* temp = PrintableTemplates[id];
			TSubclassOf<AActor> toPrint;
			temp->GetToPrintObject(toPrint);
			actor->Setup(temp->Code, toPrint);
		}
	}
}

void AVendingMachine::OnEndOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex)
{
	if (otherComp == LootCabin)
	{
		IsPulled = false;
	}
}

void AVendingMachine::CreateButtons()
{
	if (ToSpawn)
	{
		FVector startPos = FVector::ZeroVector;
		FVector newPos = startPos;
		FRotator rotation{ RootButtons->RelativeRotation };
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{

				// Spawn a button
				UWorld* world = GetWorld();
				FActorSpawnParameters spawnParams{};
				spawnParams.Owner = this;
				ABasePullButton* actor = world->SpawnActor<ABasePullButton>(ToSpawn, newPos, rotation, spawnParams);
				FAttachmentTransformRules attachmentRules{ EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false };
				actor->AttachToComponent(RootButtons, attachmentRules);
				actor->SetToggle(false);

				// Add to array
				PullButtons.Add(actor);

				// Adjust position
				newPos += RootButtons->GetForwardVector()*ButtonsPlacement.X;
			}
			newPos = startPos;
			newPos += RootButtons->GetUpVector() * ButtonsPlacement.Y * (i + 1);
		}
	}

}

int AVendingMachine::CheckPrints()
{
	for (int i = 0; i < PrintableTemplates.Num(); i++)
	{
		if (PrintableTemplates[i] != nullptr && PrintableTemplates[i]->CheckCode(ActiveButtons))
		{
			// Code correct
			return i;
		}
	}
	return -1;
}

void AVendingMachine::SetConstraint()
{
	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame1, ConstraintPositioner->RelativeLocation);

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
