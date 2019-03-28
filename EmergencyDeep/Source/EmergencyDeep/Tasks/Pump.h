

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/BaseInteractable.h"
#include "Pump.generated.h"

class UDoubleGripStaticMeshComponent;
class UDoubleGripStaticMeshComponent;
class UBoxComponent;

UCLASS()
class EMERGENCYDEEP_API APump : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APump();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnGrabReleaseController(UPhysicsHandleComponent* PhysicsHandle, USceneComponent* MotionController);

	UFUNCTION(BlueprintCallable)
		void PumpWater();

	UFUNCTION(BlueprintCallable)
		void ChargePump();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UStaticMeshComponent* PumpMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UDoubleGripStaticMeshComponent* Crank;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UBoxComponent* CrankTriggerDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UBoxComponent* CrankTriggerUp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		USceneComponent* ConstraintPositioner;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UPhysicsConstraintComponent* Constraint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CrankMaxAngle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pumping")
		bool IsPumping = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pumping")
		bool IsCharged = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pumping")
		float PumpingTime;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
		void UpdatePump(float deltaTime);

	UPROPERTY()
		float CurrentPumpTime;
	
	
};
