

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "Cork.generated.h"

class UGrippableStaticMeshComponent;
class UBoxComponent;
class AHullBreach;
class UMotionControllerComponent;
UCLASS()
class EMERGENCYDEEP_API ACork : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACork();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* motionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable)
	void GetHammered();

	UFUNCTION()
	void CloseLeak(AHullBreach* leak);

	UFUNCTION()
	void CreateLeak();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* CorkMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* HammerTriggerEnter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* HammerTriggerLeave;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Plugged = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hammer")
	int Hammered = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hammer")
	int MaxHammered = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Hammer")
	float StartOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hammer")
	float HammerSteps;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnCorking")
	float MinUnCorkTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnCorking")
	float MaxUnCorkTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UnCorking")
	float ChosenUnCorkTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UnCorking")
	float CurrentUnCorkTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnCorking")
	float UnCorkForce;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnCorked"))
		void OnCorked_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUnCorked"))
		void OnUnCorked_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnHammered"))
		void OnHammered_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPushed"))
		void OnPushed_BP();

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void UpdateUncorking(float deltaTime);

	UPROPERTY()
	AHullBreach* ClosedLeak;

	UPROPERTY()
	bool HasHammered = false;

	UPROPERTY()
	USceneComponent* MotionContr;
};
