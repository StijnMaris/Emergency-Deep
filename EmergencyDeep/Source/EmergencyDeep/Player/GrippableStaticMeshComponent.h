#pragma once

#include "CoreMinimal.h"
#include "EDGrabInterface.h"
#include "Components/StaticMeshComponent.h"
#include "GrippableStaticMeshComponent.generated.h"

class UPhysicsHandleComponent;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup = (Gripping))
class EMERGENCYDEEP_API UGrippableStaticMeshComponent : public UStaticMeshComponent, public IEDGrabInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrab(USceneComponent* MotionControllerm, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	void OnSideGrip(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool HasBeenGripped()const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool AttachComponent(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool AttachActor(USceneComponent* MotionController, USceneComponent* ToAttachActor, UPrimitiveComponent* ToSimulateComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool DettachComponent(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool DettachActor(USceneComponent* ToDettachActor, UPrimitiveComponent* ToSimulateComponent, bool IsSimPhysics);
protected:
	UPROPERTY(EditAnywhere)
		bool IsGripped;
	UPROPERTY()
		USceneComponent* MotionContr;

};
