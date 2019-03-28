

#pragma once

#include "EmergencyDeep.h"
#include "Interactables/BaseInteractable.h"
#include "PortableRadio.generated.h"

class UAudioComponent;
class UGrippableStaticMeshComponent;

UCLASS()
class EMERGENCYDEEP_API APortableRadio : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	APortableRadio();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/**
	 * \brief Start playing music. MUST be called from the server.
	 */
	void Play();

	/**
	 * \brief Stops playing music. MUST be called from the server.
	 */
	void Stop();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UChildActorComponent* PlayButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> Songs;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int32 CurrentSongIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	bool IsPlaying;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Play(int32 currentSong);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Stop();

	UFUNCTION()
	void OnSongFinished();

	UFUNCTION()
	void SetCurrentSound();

	UFUNCTION()
	void OnPlayButtonPressed();

	void PlaySong(int32 currentSong);
	void Next();
	void Previous();
};
