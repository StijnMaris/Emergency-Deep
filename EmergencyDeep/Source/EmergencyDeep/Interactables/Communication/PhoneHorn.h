

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "PhoneHorn.generated.h"

class UVoiceRecorderComponent;
class UVoiceSpeakerComponent;
class UGrippableStaticMeshComponent;
class USoundAttenuation;

UCLASS()
class EMERGENCYDEEP_API APhoneHorn : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	APhoneHorn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UVoiceRecorderComponent* RecorderComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* SpeakerPositionerComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UGrippableStaticMeshComponent* PhoneHornMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* PhoneHornCablePosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UVoiceSpeakerComponent*> SpeakerComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundAttenuation* SpeakerAttenuationSettings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Volume;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxRecordingDistance;

	protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

private:
	/**
	 * \brief Update the number of components based on the number of players connected to the session.
	 */
	void UpdateComponentCount();

	/**
	 * \brief Play all voices recorded on this phone on all other phones.
	 */
	void PlayVoices();
};
