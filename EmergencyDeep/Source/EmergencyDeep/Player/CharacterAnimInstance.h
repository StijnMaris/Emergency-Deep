

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

enum class EGrab : unsigned char;
/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterAnimInstance();

	void SetHandStates(EGrab leftHandState, EGrab rightHandState);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float IdleWalk;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LeftHandLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator LeftHandRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector RightHandLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LeftHandGrab;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RightHandGrab;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HandGripMaxMovementPerSecond;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetValueOpen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetValueCanGrab;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetValueGrab;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CrouchPercent;

protected:
	void PreUpdateAnimation(float DeltaSeconds) override;
	float GetStateTarget(EGrab grabState) const;

	float CalculateHandGrab(float deltaSeconds, float handGrip, EGrab grabState) const;

	EGrab LeftHandState;
	EGrab RightHandState;
};
