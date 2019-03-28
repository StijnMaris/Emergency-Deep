#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseInteractable.h"
#include "BaseProjectile.generated.h"

class UGrippableStaticMeshComponent;
class UBoxComponent;
class AHarpoonDispenser;

UCLASS()
class EMERGENCYDEEP_API ABaseProjectile : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent *ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent *HeadTrigger;

	UPROPERTY(VisibleAnywhere)
	AHarpoonDispenser *Dispenser;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileDamage;

	UPROPERTY()
	bool bReadyForDelete;

private:
	FVector Velocity;
	bool bInFlight;
};
