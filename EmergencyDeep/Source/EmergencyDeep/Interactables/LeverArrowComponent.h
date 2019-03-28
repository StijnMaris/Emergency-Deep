#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "LeverArrowComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API ULeverArrowComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeverArrowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetInputAngle(float angle);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MinInputAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxInputAngle;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MinOutputAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxOutputAngle;

private:
	void CalculateOutputAngle();

private:
	float InputAngle;
	float OutputAngle;

	float MiddleInputAngle;
	float MiddleOutputAngle;
};
