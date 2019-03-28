#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/Actor.h"
#include "EDPlayerHand.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGrab : uint8
{
	Open 		UMETA(DisplayName = "Open"),
	CanGrab 	UMETA(DisplayName = "CanGrab"),
	Grab		UMETA(DisplayName = "Grab")
};

class UMotionControllerComponent;
class USplineComponent;
class UArrowComponent;
class USphereComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class USplineMeshComponent;
class UTeleportationComponent;
class UPhysicsHandleComponent;

UCLASS()
class EMERGENCYDEEP_API AEDPlayerHand : public AActor
{
	GENERATED_BODY()

public:

	AEDPlayerHand();

	void Initialize(EControllerHand hand);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void GrabObject();
	void SideGripObject();
	void DropObject();

	void DropObject_Implementation();
	void ActivateTeleport(bool large);
	void DeActivateTeleport();

	EGrab GetGrabState() const;

	UFUNCTION(BlueprintCallable)
	FVector GetForward() const;

	UFUNCTION(BlueprintCallable)
	FVector GetRight() const;

	UFUNCTION(BlueprintCallable)
	AActor* GetAttachedActor() const;

	UFUNCTION(BlueprintCallable)
	void ResetHandMesh();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRootComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HandMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArcDirectionComponent;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* ArcSplineComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* GrabSphereComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArcEndpointComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TeleportLocationMarkerComponent;

	UPROPERTY(VisibleAnywhere)
	UTeleportationComponent* TeleportationComponent;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandleComponent;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GrabObject();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_GrabObject();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SideGripObject();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SideGripObject();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DropObject();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_DropObject();


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated)
	EControllerHand Hand;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated)
	EGrab GrabState;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* AttachedActor;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UPrimitiveComponent* AttachedComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool WantsToGrip;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsActorGrip;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector GrabLocation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsTeleportPressed = false;

private:
	void SetupCollision() const;
	void SetupTeleportation() const;

	void UpdateTeleportation() const;
	void UpdateGrab();
	void UpdateHandAnimation();

	UObject* GetObjectNearHand();

	UPROPERTY()
	FVector OriginalHandLocation;
};
