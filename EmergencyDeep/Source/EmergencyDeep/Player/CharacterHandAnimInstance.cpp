

#include "CharacterHandAnimInstance.h"

UCharacterHandAnimInstance::UCharacterHandAnimInstance()
{
	HandGrip = 0.0f;
	HandState = EGrab::Open;
}

void UCharacterHandAnimInstance::PreUpdateAnimation(float DeltaSeconds)
{
	Super::PreUpdateAnimation(DeltaSeconds);

	const float currentMaxAddedMovement = HandGripMaxMovementPerSecond * DeltaSeconds;
	const float target = GetStateTarget();

	float movement = FMath::Clamp(currentMaxAddedMovement, 0.0f, abs(target - HandGrip));

	if(HandGrip > target)
	{
		movement *= -1.0f;
	}

	HandGrip += movement;
}

float UCharacterHandAnimInstance::GetStateTarget() const
{
	switch (HandState) 
	{ 
		case EGrab::Open: return TargetValueOpen;
		case EGrab::CanGrab: return TargetValueCanGrab;
		case EGrab::Grab: return TargetValueGrab;
		default:; 
	}
	return 0.0f;
}

void UCharacterHandAnimInstance::SetHandState(EGrab newState)
{
	HandState = newState;
}
