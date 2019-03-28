#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EDGrabInterface.generated.h"

class UPhysicsHandleComponent;

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UEDGrabInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class EMERGENCYDEEP_API IEDGrabInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	// Event triggered on the interfaced object when grip is released
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrabReleaseController(UPhysicsHandleComponent* PhysicsHandle, USceneComponent* MotionController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	void OnSideGrip(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool HasBeenGripped() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool AttachComponent(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool AttachActor(USceneComponent* MotionController, USceneComponent* ToAttachActor, UPrimitiveComponent* ToSimulateComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool DettachComponent(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool DettachActor(USceneComponent* ToDettachActor, UPrimitiveComponent* ToSimulateComponent, bool IsSimPhysics);
};
