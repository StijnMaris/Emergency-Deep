#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "NewPipe.generated.h"

class UGrippableStaticMeshComponent;
class UMotionControllerComponent;
//class UBoxComponent;

UCLASS()
class EMERGENCYDEEP_API ANewPipe : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	ANewPipe();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere,Replicated)
	UGrippableStaticMeshComponent* Pipe;

	/*UPROPERTY(VisibleAnywhere,Replicated)
	UBoxComponent* TriggerBoxLeft;

	UPROPERTY(VisibleAnywhere,Replicated)
	UBoxComponent* TriggerBoxRight;*/

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	// Variables
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "New Pipe")
	USceneComponent* HandLeft;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "New Pipe")
	USceneComponent* HandRight;
};
