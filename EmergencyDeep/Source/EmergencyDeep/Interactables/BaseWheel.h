

#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseInteractable.h"
#include "BaseWheel.generated.h"

class UGrippableStaticMeshComponent;

UENUM(BlueprintType)
enum class ERotationAxis : uint8
{
	RA_X UMETA(DisplayName = "X"),
	RA_Y UMETA(DisplayName = "Y"),
	RA_Z UMETA(DisplayName = "Z"),
};

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API ABaseWheel : public ABaseInteractable
{
	GENERATED_BODY()

public:
	ABaseWheel();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsValveOn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* WheelBase;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* ValveMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* ConstraintPositioner;

	UPROPERTY(BlueprintReadOnly)
	UPhysicsConstraintComponent* Constraint;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bLimitValveRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ValveMaxAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ERotationAxis RotationAxis;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* OnTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* OffTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* WheelTriggerBox;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnValveRotated"))
	void OnValveRotated_BP(float deltaAngle, float newAngle);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnValveRotatedToggle"))
	void OnValveRotatedToggle_BP(bool newStatus);

private:
	float LastFrameRotation;
	bool bOnOff = false;
};
