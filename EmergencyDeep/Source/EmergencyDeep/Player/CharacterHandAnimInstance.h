

#pragma once

#include "EmergencyDeep.h"
#include "Animation/AnimInstance.h"
#include "EDPlayerHand.h"
#include "CharacterHandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API UCharacterHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterHandAnimInstance();

	void SetHandState(EGrab newState);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HandGripMaxMovementPerSecond;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetValueOpen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetValueCanGrab;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetValueGrab;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float HandGrip;

protected:
	void PreUpdateAnimation(float DeltaSeconds) override;

private:
	float GetStateTarget() const;

	EGrab HandState;
};
