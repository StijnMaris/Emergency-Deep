#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseInteractable.h"
#include "BaseGuideBookPage.generated.h"

class UGrippableStaticMeshComponent;
class UPhysicsConstraintComponent;
class USceneComponent;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API ABaseGuideBookPage : public ABaseInteractable
{
	GENERATED_BODY()

	ABaseGuideBookPage();
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	UGrippableStaticMeshComponent *PageMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent *ConstraintPositioner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsConstraintComponent *Constraint;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);
};
