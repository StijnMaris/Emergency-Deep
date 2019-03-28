#include "LeverArrowComponent.h"

// Sets default values for this component's properties
ULeverArrowComponent::ULeverArrowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MinInputAngle = -20.0f;
	MaxInputAngle = 20.0f;
	MinOutputAngle = -90.0f;
	MaxOutputAngle = 90.0f;

	InputAngle = 0.0f;
	OutputAngle = 0.0f;
}

// Called when the game starts
void ULeverArrowComponent::BeginPlay()
{
	Super::BeginPlay();

	MiddleInputAngle = MaxInputAngle / 2.0f + MinInputAngle / 2.0f;
	MiddleOutputAngle = MaxOutputAngle / 2.0f + MinOutputAngle / 2.0f;
}


// Called every frame
void ULeverArrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateOutputAngle();

	FQuat rotation(FRotator::MakeFromEuler(FVector(0.0f, OutputAngle, 0.0f)));
	SetRelativeRotation(rotation);
}

void ULeverArrowComponent::SetInputAngle(float angle)
{
	InputAngle = angle;
}

void ULeverArrowComponent::CalculateOutputAngle()
{
	float rawAngle = InputAngle - MiddleInputAngle;
	float diviser;
	if (InputAngle < MiddleInputAngle)
	{
		diviser = MiddleInputAngle - MinInputAngle;
	}
	else if (InputAngle > MiddleInputAngle)
	{
		diviser = MaxInputAngle - MiddleInputAngle;
	}
	else
	{
		OutputAngle = MiddleOutputAngle;
		return;
	}

	float scaler = rawAngle / diviser;

	if (scaler < -1.0f) scaler = -1.0; else if (scaler > 1.0f) scaler = 1.0f;

	if (scaler < 0)
	{
		OutputAngle = FMath::Abs<float>(scaler) * MinOutputAngle;
	}
	else
	{
		OutputAngle = FMath::Abs<float>(scaler) * MaxOutputAngle;
	}
}
