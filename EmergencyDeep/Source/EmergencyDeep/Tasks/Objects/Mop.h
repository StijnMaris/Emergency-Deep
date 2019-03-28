#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "Mop.generated.h"

class UGrippableStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class EMERGENCYDEEP_API AMop : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()

public:
	AMop();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere,Replicated)
	UGrippableStaticMeshComponent* Mop;

	UPROPERTY(VisibleAnywhere,Replicated)
	USceneComponent* CleaningPos;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Flames;

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BluePrintCallable)
	void CatchFire();

	UFUNCTION(BluePrintCallable)
	void StopFire();

	/*UFUNCTION(reliable, server, WithValidation)
    void Server_StopFire();*/

	UFUNCTION(BluePrintCallable)
	FVector GetCleaningPos();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Mop Sound")
	void OnCatchFire();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Mop Sound")
	void OnStopFire();

	// Variables
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mop", Replicated)
	bool IsOnFire;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mop", Replicated)
	float TimeOnFire;
};
