#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseInteractable.h"
#include "HarpoonGun.generated.h"

class UStaticMeshComponent;
class UGrippableStaticMeshComponent;
class ABaseProjectile;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AHarpoonGun : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	AHarpoonGun();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnShoot"))
	void OnShoot_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnOutOfAmmo"))
	void OnOutOfAmmo_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInternalReloadStart"))
	void OnInternalReloadStart_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInternalReloadStart"))
	void OnInternalReloadEnd_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnAmmoInsertion"))
	void OnAmmoInsertion_BP();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TryShoot();
	void TryShoot_Implementation();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* HarpoonGunCabin;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* HarpoonGunHandle;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* HarpoonGunBarrel;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* HarpoonGunBarrelEnd;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* ConstraintPositioner;

	UPROPERTY(BlueprintReadOnly)
	UPhysicsConstraintComponent* Constraint;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent *AmmoLoadingArea;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> HarpoonActor;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float FireRate;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	uint8 LoadedAmmo;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 MaxLoadedAmmo;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsGrabbedByPlayer;

private:
	float ShootTimer;
	bool CanShoot;
};
