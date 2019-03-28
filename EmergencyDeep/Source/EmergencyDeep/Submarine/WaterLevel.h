

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterLevel.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AHullBreach;
class UPostProcessComponent;
class APump;

UCLASS()
class EMERGENCYDEEP_API AWaterLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterLevel();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetWaterPercentage();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* WaterPlaneUp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* WaterPlaneUnder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AHullBreach*> HullBreaches;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<APump*> Pumps;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UPostProcessComponent* PostProcessComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Level;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaterIncreaseRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaterDecreaseRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaterLevelMax;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void UpdateWater(float deltaTime);

	UFUNCTION()
	void UpdatePlane();
	
	UPROPERTY()
	FVector StartPosition;

	
	
};
