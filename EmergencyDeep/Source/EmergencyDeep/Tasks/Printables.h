

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "Printables.generated.h"

class UGrippableStaticMeshComponent;
class UMotionControllerComponent;

UCLASS()
class EMERGENCYDEEP_API APrintables : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrintables();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Properties
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* Package;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsGripped;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Shake;

	UPROPERTY(EditAnywhere)
	float ShakeLimit = 100.0f;

	UPROPERTY(EditAnywhere)
	float ShakeSensitive = 0.9f;

	UPROPERTY(EditAnywhere)
	int Code;

	UPROPERTY(EditAnywhere)
	bool IsTemplate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ToSpawn;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* motionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION()
	void Setup(int vendingCode, TSubclassOf<AActor> spawn, float shakeLim = 100.0f, float sensitivity = 0.9f);

	UFUNCTION()
	void GetToPrintObject(TSubclassOf<AActor>& object);

	UFUNCTION()
	bool CheckCode(const TArray<int>& vendingCode);

	UFUNCTION(BlueprintCallable)
	void SetVisible(bool visible);

	UFUNCTION(BlueprintCallable)
	void Unpack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	USceneComponent* MotionContr;

	FVector PrevPos;
	FVector PrevVelocity;

	void UpdateShake(float DeltaTime);
	
	
};
