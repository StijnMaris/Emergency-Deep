

#pragma once

#include "EmergencyDeep.h"
#include "BaseInteractable.h"
#include "BaseLever.generated.h"

class UBoxComponent;
class UGrippableStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTopTriggerEnter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTopTriggerLeave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBotTriggerEnter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBotTriggerLeave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandleMoveBeginDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandleMoveEndDelegate);

UENUM(BlueprintType, meta = (DisplayName = "ELeverState"))
enum class ELeverState : uint8
{
	CENTER	UMETA(DisplayName = "Center"),
	TOP		UMETA(DisplayName = "Top"),
	BOTTOM	UMETA(DisplayName = "Bottom")
};

UCLASS()
class EMERGENCYDEEP_API ABaseLever : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	ABaseLever();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnTopTriggerBegin();
	virtual void OnTopTriggerEnd();
	virtual void OnBottomTriggerBegin();
	virtual void OnBottomTriggerEnd();
	virtual void OnHandleMoveBegin();
	virtual void OnHandleMoveEnd();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UPROPERTY(BlueprintAssignable)
	FTopTriggerEnter OnTopTriggerEnterDelegate;

	UPROPERTY(BlueprintAssignable)
	FTopTriggerLeave OnTopTriggerLeaveDelegate;

	UPROPERTY(BlueprintAssignable)
	FBotTriggerEnter OnBotTriggerEnterDelegate;

	UPROPERTY(BlueprintAssignable)
	FBotTriggerLeave OnBotTriggerLeaveDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHandleMoveBeginDelegate OnHandleMoveBeginDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHandleMoveEndDelegate OnHandleMoveEndDelegate;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* LeverMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* HandleMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* HandleTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* TopTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* BottomTriggerBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* GrabReleaseCapsuleComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ELeverState LeverState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LeverMinAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LeverMaxAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinMovementEventTriggerValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BaseOffset;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTopTriggerBegin"))
	void OnTopTriggerBegin_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTopTriggerEnd"))
	void OnTopTriggerEnd_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnBottomTriggerBegin"))
	void OnBottomTriggerBegin_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnBottomTriggerEnd"))
	void OnBottomTriggerEnd_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnHandleMoveBegin"))
	void OnHandleMoveBegin_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnHandleMoveEnd"))
	void OnHandleMoveEnd_BP();
	
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void OnLeaveGrabSphere(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	void UpdateLeverRotation();

	UPROPERTY()
	USceneComponent* CurrentInteractor;

	float AccumulatedMovement;
	bool HasBeginMoved;
};
