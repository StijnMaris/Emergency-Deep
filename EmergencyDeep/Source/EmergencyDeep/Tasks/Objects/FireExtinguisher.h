#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "FireExtinguisher.generated.h"

class UGrippableStaticMeshComponent;
class UParticleSystemComponent;
class UBoxComponent;
class UCableComponent;

UCLASS()
class EMERGENCYDEEP_API AFireExtinguisher : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()

public:
	AFireExtinguisher();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere,Replicated)
	UGrippableStaticMeshComponent* Base;

	//UPROPERTY(VisibleAnywhere)
	//UCableComponent* Cable;

	UPROPERTY(VisibleAnywhere,Replicated)
	UChildActorComponent* Tube;

	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSideGrip(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);
	
	// Variables
	UPROPERTY(VisibleAnywhere,Replicated)
	bool IsGripped;

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Fire Extinguisher Sound")
	void OnActivate();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Fire Extinguisher Sound")
	void OnDeactivate();
};
