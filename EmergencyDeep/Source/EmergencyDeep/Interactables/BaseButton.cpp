#include "BaseButton.h"

ABaseButton::ABaseButton()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	ButtonBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBaseMesh"));
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonTrigger"));

	RootComponent = ButtonBaseMesh;
	ButtonMesh->SetupAttachment(RootComponent);
	ButtonTrigger->SetupAttachment(RootComponent);

	ButtonBaseMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	ButtonMesh->SetCollisionProfileName(TEXT("Interactable"));
	ButtonTrigger->SetCollisionProfileName(TEXT("InteractableTrigger"));

	ButtonTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseButton::OnBeginOverlapTrigger);
	ButtonTrigger->OnComponentEndOverlap.AddDynamic(this, &ABaseButton::OnEndOverlapTrigger);

	ButtonMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseButton::OnBeginOverlapMesh);
	ButtonMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseButton::OnEndOverlapMesh);

	LinearLimit = 2.0f;
	Stiffness = 5.0f;
}

void ABaseButton::BeginPlay()
{
	Super::BeginPlay();

	InitialButtonLocation = ButtonMesh->RelativeLocation;
}

void ABaseButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If a hand is pushing the button
	if (OverlappedComponent)
	{
		// Calculate how far we pressed the button.
		const FVector buttonInvTransDir = ButtonMesh->GetComponentTransform().InverseTransformVectorNoScale(InitialOverlapLocation);
		const FVector overlapInvTransDir = ButtonMesh->GetComponentTransform().InverseTransformVectorNoScale(OverlappedComponent->GetComponentLocation());
		const FVector result = buttonInvTransDir - overlapInvTransDir;
		const float clampedResult = FMath::Clamp(result.Z, 0.0f, LinearLimit);

		// If we are pressing and not releasing set the amount currently pressed
		if (clampedResult > PressedAmount)
		{
			PressedAmount = clampedResult;
		}
	}
	else
	{
		// Makes the button go up again after being pressed,
		PressedAmount -= Stiffness * DeltaTime;
		PressedAmount = FMath::Clamp(PressedAmount, 0.0f, LinearLimit);
	}

	// Set the relative location based on how much we pressed the button.
	const FVector newLocation = InitialButtonLocation - FVector{ 0.0f, 0.0f, PressedAmount };
	ButtonMesh->SetRelativeLocation(newLocation, false, nullptr, ETeleportType::None);
}

void ABaseButton::OnButtonPressed()
{
	IsPressed = true;

	OnButtonPressed_BP();

	OnButtonPressedDelegate.Broadcast();
}

void ABaseButton::OnButtonReleased()
{
	IsPressed = false;

	OnButtonReleased_BP();

	OnButtonReleasedDelegate.Broadcast();
}

void ABaseButton::OnBeginOverlapMesh(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(!OverlappedComponent && otherComp != ButtonTrigger && otherComp->GetCollisionProfileName() == FName{"PlayerHands"})
	{
		OverlappedComponent = otherComp;
		InitialOverlapLocation = otherComp->GetComponentLocation();
	}
}

void ABaseButton::OnEndOverlapMesh(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if(otherComp == OverlappedComponent)
	{
		OverlappedComponent = nullptr;
	}
}

void ABaseButton::OnBeginOverlapTrigger(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(otherComp == ButtonMesh)
	{
		OnButtonPressed();
	}
}

void ABaseButton::OnEndOverlapTrigger(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherComp == ButtonMesh)
	{
		OnButtonReleased();
	}
}


