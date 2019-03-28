#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "FireExtinguisherTube.generated.h"


class UGrippableStaticMeshComponent;
class UParticleSystemComponent;
class UBoxComponent;

UCLASS()
class EMERGENCYDEEP_API AFireExtinguisherTube : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	AFireExtinguisherTube();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere,Replicated)
	UGrippableStaticMeshComponent* Tube;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Smoke;

	// Functions 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION( BlueprintCallable)
	void Activate();

	UFUNCTION( BlueprintCallable)
	void Deactivate();

	// Variables
	UPROPERTY(VisibleAnywhere,Replicated)
	bool IsGripped;

	UPROPERTY(VisibleAnywhere,Replicated)
	bool IsActive;

	private:
	// Overlapping
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
