#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SliderPosition.generated.h"

class UBoxComponent;
class ABaseSlider;

UCLASS()
class EMERGENCYDEEP_API ASliderPosition : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ASliderPosition();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 SliderPositionValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ABaseSlider *OwnerSlider;

	UBoxComponent *OwnerTrigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumberOfSteps;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};
