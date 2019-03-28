

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mop.h"
#include "OilSpill.generated.h"

UENUM(BlueprintType)
enum class EGrowState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Growing 	UMETA(DisplayName = "Growing"),
	Shrinking	UMETA(DisplayName = "Shrinking")
};

class UDecalComponent;
class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class EMERGENCYDEEP_API AOilSpill : public AActor
{
	GENERATED_BODY()
	
public:	
	AOilSpill();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere,Replicated)
	UDecalComponent* OilSpill;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Flames;

	// Functions
	UFUNCTION(BluePrintCallable)
	void CatchFire();

	UFUNCTION(BluePrintCallable)
	void StopFire();

	UFUNCTION(BluePrintCallable)
	void GrowAndShrinkFire();

	UFUNCTION(BluePrintCallable)
	float UpdateScale(float DeltaTime,float scale);

	UFUNCTION(BluePrintCallable)
	void GrowAndShrinkOil();

	UFUNCTION(BluePrintCallable)
	void CheckIfMopIsCleening();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Oil Spill Sound")
	void OnCatchFire();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Oil Spill Sound")
	void OnStopFire();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Oil Spill Sound")
	void OnCleaningOil();

	// Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OilSpill")
	float OriginalGrowSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OilSpill")
	float CleaningSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	float GrowSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	float OilScale;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	float FireScale;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	float CatchFireDelay;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	float OxigenDepletionRate;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	AMop* Mop;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	FVector LastPos;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	bool IsOnFire;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	bool IsValveOpen;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	EGrowState OilGrowState;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OilSpill",Replicated)
	EGrowState FireGrowState;

private:
	// Overlapping
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
	
};
