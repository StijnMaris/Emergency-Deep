#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseInteractable.h"
#include "BaseSlider.generated.h"

class UBoxComponent;
class UGrippableStaticMeshComponent;
class USliderPositionComponent;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API ABaseSlider : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	ABaseSlider();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* SliderMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* HandleMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* HandleTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* TopTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* BottomTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* GrabReleaseCapsuleComponent;

	/**
	 * \brief Value of the slider between 0.0f and 1.0f.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CurrentSliderValue;

	/**
	 * \brief The bottom position of the slider. Corresponds with 0.0f slider value.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SliderMinPosition;

	/**
	 * \brief The top position of the slider. Corresponds with 1.0f slider value.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SliderMaxPosition;

	/**
	 * \brief Set the slider to a specific value.
	 * \param value slider value between 0.0f and 1.0f.
	 */
	UFUNCTION(BlueprintCallable)
	void ChangeSliderValue(float value);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnValueChanged"))
	void OnValueChanged_BP(float newValue);

	UFUNCTION()
	void OnLeaveGrabSphere(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

private:
	void BeginPlay() override;
	void UpdateSliderLocation();
	void Tick(float DeltaTime) override;


	USceneComponent* CurrentInteractor;
	FVector InitialSliderLocation;

};
