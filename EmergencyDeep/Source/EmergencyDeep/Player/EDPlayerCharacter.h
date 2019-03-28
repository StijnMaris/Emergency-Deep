// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/Character.h"
#include "EDPlayerCharacter.generated.h"

class AEDPlayerHand;
class UCameraComponent;
class USceneComponent;
class UStaticMeshComponent;
class UPostProcessComponent;
class UMaterialInterface;
class UCurveFloat;
class UPlayerVoiceComponent;
class UMotionControllerComponent;

USTRUCT()
struct FMotionControllerReplicationData
{
	GENERATED_BODY()

	void Set(const FVector& position, const FRotator& rotation)
	{
		HandPosition = position;
		HandRotation = FVector{ rotation.Pitch, rotation.Yaw, rotation.Roll };
	}

	FVector GetPosition() const
	{
		return HandPosition;
	}

	FRotator GetRotation() const
	{
		return FRotator{ HandRotation.X, HandRotation.Y, HandRotation.Z };
	}

private:
	UPROPERTY()
	FVector_NetQuantize100 HandPosition;

	UPROPERTY()
	FVector_NetQuantize100 HandRotation;
};

UCLASS()
class EMERGENCYDEEP_API AEDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEDPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SpawnMotionControllers();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* VrCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* VrOrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerVoiceComponent* PlayerVoiceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* PlayerNameComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HandsReplicated)
	AEDPlayerHand* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HandsReplicated)
	AEDPlayerHand* RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMotionControllerComponent* LeftHandMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMotionControllerComponent* RightHandMotionController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEDPlayerHand> PlayerHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFpsMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OverrideFpsMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HeadHeight;

	void MoveForward(float throttle);
	void MoveRight(float throttle);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_HandsReplicated)
	int32 NetOwnerId;

	void Crouch(bool bClientSimulation) override;

protected:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_HandsReplicated();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateMotionControllers(const FMotionControllerReplicationData& leftHandData, const FMotionControllerReplicationData& rightHandData);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateVROrigin(const FVector_NetQuantize10& vrOriginLocation);

	void SetMeshRotation(const FRotator& rotation);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateMeshRotation(const FRotator& rotation);

private:
	void SetUpPlayerHeight();
	void SetupBlinker();
	void UpdateMotionControllers();
	void UpdateAnimations() const;

	void UpdateVrOrigin();
	void UpdateBlinkers() const;
	void UpdatePlayerCapsule() const;
	void UpdateVRRotations();

	FVector2D GetBlinkerCenter() const;

	UPROPERTY()
	UPostProcessComponent* PostProcessComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* BlinkMaterialBase;

	UPROPERTY(EditAnywhere)
	UCurveFloat* RadiusVsVelocity;

	UPROPERTY(EditAnywhere)
	bool UseBlinker;

	bool HasInitMotionControllers;
};
