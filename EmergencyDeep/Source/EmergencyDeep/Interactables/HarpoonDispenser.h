#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarpoonDispenser.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ABaseProjectile;

/**
 * 
 */
UCLASS()
class EMERGENCYDEEP_API AHarpoonDispenser : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	AHarpoonDispenser();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Dispense();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDispense"))
	void OnDispense_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDispenseMaxed"))
	void OnDispenseMaxed_BP();

	UFUNCTION(BlueprintCallable)
	void ReduceSpawnCount();

public:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent *DispenserMesh;

	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	USceneComponent *DispensePositioner;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> ToSpawn;
	
	UPROPERTY(EditAnywhere)
	uint32 SpawnLimit;

private:
	uint32 SpawnCounter;
};
