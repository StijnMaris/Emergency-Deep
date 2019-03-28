#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldControlComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UWorldControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartMove();

	UFUNCTION(BlueprintCallable)
	void StopMove();

	UFUNCTION(BlueprintCallable)
	void Rotate(float angle);

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UPROPERTY(EditAnywhere)
	float Acceleration;

	UPROPERTY(EditAnywhere)
	float Deceleration;

private:
	bool IsMoving = false;
	float CurrentSpeed;
};
