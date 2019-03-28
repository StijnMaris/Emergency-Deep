#include "BaseSlider.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "SliderPosition.h"

ABaseSlider::ABaseSlider()
{
	PrimaryActorTick.bCanEverTick = true;

	SliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SliderMesh"));
	HandleMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("HandleMesh"));
	HandleTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HandleTriggerBox"));
	GrabReleaseCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GrabReleaseCapsuleComponent"));

	RootComponent = SliderMesh;
	HandleMesh->SetupAttachment(SliderMesh);
	HandleTriggerBox->SetupAttachment(HandleMesh);
	GrabReleaseCapsuleComponent->SetupAttachment(HandleMesh);

	GrabReleaseCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseSlider::OnLeaveGrabSphere);

	CurrentSliderValue = 0.0f;
	SliderMinPosition = -10.0f;
	SliderMaxPosition = 10.0f;
}

void ABaseSlider::BeginPlay()
{
	Super::BeginPlay();

	check(SliderMinPosition < SliderMaxPosition)

	InitialSliderLocation = HandleMesh->RelativeLocation;
}

void ABaseSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSliderLocation();
}

void ABaseSlider::UpdateSliderLocation()
{
	if (!CurrentInteractor) return;

	FTransform sliderTransform = GetActorTransform();
	const FVector interactorLocation = CurrentInteractor->GetComponentLocation();

	const FVector inverseLocation = sliderTransform.InverseTransformPosition(interactorLocation);
	const FVector difference = InitialSliderLocation - inverseLocation;
	const float sliderLength = SliderMaxPosition - SliderMinPosition;

	const float clampedPosition = FMath::Clamp(difference.Y, SliderMinPosition, SliderMaxPosition);
	//CurrentSliderValue = 1.0f + (SliderMinPosition + clampedPosition) / sliderLength;

	HandleMesh->SetRelativeLocation(InitialSliderLocation + FVector{ 0.0f, -clampedPosition , 0.0f });
}

void ABaseSlider::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	CurrentInteractor = MotionController;
}

void ABaseSlider::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	OnValueChanged_BP(CurrentSliderValue);
	CurrentInteractor = nullptr;
}

void ABaseSlider::OnLeaveGrabSphere(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherComp == CurrentInteractor)
	{
		CurrentInteractor = nullptr;
	}
}

void ABaseSlider::ChangeSliderValue(const float value)
{
	check(value >= 0.0f && value <= 1.0f)

	CurrentSliderValue = value;
	const float sliderLength = SliderMaxPosition - SliderMinPosition;
	const float newPosition = SliderMinPosition + CurrentSliderValue * sliderLength;

	//HandleMesh->SetRelativeLocation(InitialSliderLocation + FVector{ 0.0f, -newPosition , 0.0f });
}