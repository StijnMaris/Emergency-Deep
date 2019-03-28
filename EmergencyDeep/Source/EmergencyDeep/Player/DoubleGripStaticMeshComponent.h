#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "EDGrabInterface.h"
#include "DoubleGripStaticMeshComponent.generated.h"


class AEDPlayerHand;
class UPhysicsHandleComponent;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup = (Gripping))
class EMERGENCYDEEP_API UDoubleGripStaticMeshComponent : public UStaticMeshComponent, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrab(USceneComponent* MotionControllerm, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	void OnGrabReleaseController(UPhysicsHandleComponent* PhysicsHandle, USceneComponent* MotionController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool AttachComponent(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool AttachActor(USceneComponent* MotionController, USceneComponent* ToAttachActor, UPrimitiveComponent* ToSimulateComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool DettachComponent(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VRGripInterface")
	bool DettachActor(USceneComponent* ToDettachActor, UPrimitiveComponent* ToSimulateComponent, bool IsSimPhysics);

	UFUNCTION(BlueprintCallable)
	bool HasGrabbed(bool bothHands);

	UFUNCTION()
	void UpdateGrip(float deltaSeconds);

	UPROPERTY(EditAnywhere)
	float MaxGripDistance;

	UPROPERTY(EditAnywhere)
	float VisualResistencePercent;

	
private:
	
	UPROPERTY()
		AEDPlayerHand* PrimaryHand;

	UPROPERTY()
		AEDPlayerHand* SecondaryHand;

	UPROPERTY()
		float Resistence;

	UPROPERTY()
		FVector PrimaryGrabbedLocation;

	UPROPERTY()
		FVector RelativeOffset;

	UPROPERTY()
		FVector StartOrientation;

	UPROPERTY()
		bool IsGrabbed;

	UPROPERTY()
		bool UsesBothHands;
	
};
