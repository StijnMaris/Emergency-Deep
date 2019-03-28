

#include "CharacterAnimInstance.h"
#include "Player/EDPlayerCharacter.h"
#include "Player/EDPlayerHand.h"
#include "MotionControllerComponent.h"
#include "Player/EDPlayerHand.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	LeftHandState = EGrab::Open;
	RightHandState = EGrab::Open;
	IdleWalk = 0.0f;
	LeftHandGrab = 0.0f;
	RightHandGrab = 0.0f;
	LeftHandLocation = FVector{ 0.0f };
	RightHandLocation = FVector{ 0.0f };
	LeftHandRotation = FRotator{ 0.0f };
	RightHandRotation = FRotator{ 0.0f };
}

void UCharacterAnimInstance::SetHandStates(EGrab leftHandState, EGrab rightHandState)
{
	LeftHandState = leftHandState;
	RightHandState = rightHandState;
}

void UCharacterAnimInstance::PreUpdateAnimation(float DeltaSeconds)
{
	Super::PreUpdateAnimation(DeltaSeconds);

	AEDPlayerCharacter* player = Cast<AEDPlayerCharacter>(GetSkelMeshComponent()->GetOwner());
	if(player)
	{
		const float speed = player->GetVelocity().Size();
		const float maxSpeed = player->GetCharacterMovement()->GetMaxSpeed();

		IdleWalk = speed / maxSpeed * 100;

		if (player->LeftHand && player->RightHand)
		{
			LeftHandLocation = player->LeftHand->GetActorLocation();
			RightHandLocation = player->RightHand->GetActorLocation();
			LeftHandRotation = player->LeftHand->GetActorRotation();
			RightHandRotation = player->RightHand->GetActorRotation();

			LeftHandGrab = CalculateHandGrab(DeltaSeconds, LeftHandGrab, LeftHandState);
			RightHandGrab = CalculateHandGrab(DeltaSeconds, RightHandGrab, RightHandState);
		}
	}
}

float UCharacterAnimInstance::CalculateHandGrab(const float deltaSeconds, const float handGrip, const EGrab grabState) const
{
	const float currentMaxAddedMovement = HandGripMaxMovementPerSecond * deltaSeconds;
	const float target = GetStateTarget(grabState);

	float movement = FMath::Clamp(currentMaxAddedMovement, 0.0f, abs(target - handGrip));

	if (handGrip > target)
	{
		movement *= -1.0f;
	}

	return handGrip + movement;
}

float UCharacterAnimInstance::GetStateTarget(const EGrab grabState) const
{
	switch (grabState)
	{
	case EGrab::Open: return TargetValueOpen;
	case EGrab::CanGrab: return TargetValueCanGrab;
	case EGrab::Grab: return TargetValueGrab;
	default:;
	}
	return 0.0f;
}