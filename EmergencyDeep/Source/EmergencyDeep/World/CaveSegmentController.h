#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaveFoliage.h"
#include "CaveSegmentController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UCaveSegmentController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCaveSegmentController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Move(float Speed);
	
	UFUNCTION()
	void UpdateRotation(float Rotation);

	UFUNCTION()
	void AddFoliage(UCaveFoliage *foliage);

private:
	bool WasAdded;
	TArray<UCaveFoliage*> AttachedFoliage;
};
