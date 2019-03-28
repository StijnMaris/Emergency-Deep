

#pragma once

#include "EmergencyDeep.h"
#include "BaseInteractable.h"
#include "BasePullButton.generated.h"

class UGrippableStaticMeshComponent;

UCLASS()
class EMERGENCYDEEP_API ABasePullButton : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	ABasePullButton();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnPullButtonPulled();
	virtual void OnPullButtonReleased();
	virtual void OnPullButtonPulledMovedBegin();
	virtual void OnPullButtonPulledMovedEnd();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);


	UFUNCTION(BlueprintCallable)
	void SetToggle(bool toggled);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* ButtonBaseMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* ButtonMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* ConstraintPositioner;

	UPROPERTY(BlueprintReadOnly)
	UPhysicsConstraintComponent* Constraint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PullButton")
	float LinearLimit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PullButton")
	float MotorForce;

	UPROPERTY(BlueprintReadOnly)
	bool IsPulled;

	UPROPERTY(BlueprintReadOnly)
	bool Toggled;


protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPullButtonPulled"))
	void OnPullButtonPulled_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPullButtonReleased"))
	void OnPullButtonReleased_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPullButtonPulledMovedBegin"))
	void OnPullButtonPulledMovedBegin_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPullButtonPulledMovedEnd"))
	void OnPullButtonPulledMovedEnd_BP();

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UPROPERTY()
	float PullDistance;

	bool WasPulled;
};
