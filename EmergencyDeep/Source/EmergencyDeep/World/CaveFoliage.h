

#pragma once

#include "EmergencyDeep.h"
#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "CaveFoliage.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Foliage, Blueprintable)
class EMERGENCYDEEP_API UCaveFoliage : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()

	UCaveFoliage();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Move(float Speed);
	void Move_Implementation(float Rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateRotation(float Rotation);
	void UpdateRotation_Implementation(float Rotation);

protected:
	virtual void BeginPlay() override;
};
