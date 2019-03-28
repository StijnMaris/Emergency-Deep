#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "Hammer.generated.h"



UCLASS()
class EMERGENCYDEEP_API AHammer : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	AHammer();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere,Replicated)
	UGrippableStaticMeshComponent* Hammer;

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);
	
};
