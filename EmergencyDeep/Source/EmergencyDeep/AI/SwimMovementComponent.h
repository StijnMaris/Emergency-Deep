#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwimMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API USwimMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwimMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetTargetLocation(const FVector &Target);

	UFUNCTION()
	const FVector &GetVelocity() const;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UPROPERTY(EditAnywhere)
	float Acceleration;

	UPROPERTY(EditAnywhere)
	float Deceleration;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

private:
	FVector Velocity;
	FVector DesiredVelocity;
	FVector TargetSwimLocation;
	float CurrentSpeed;
	float DesiredSpeed;

private:
	void UpdateDesiredVelocity();
	void UpdateVelocity(float DeltaTime);
	void UpdateLocation(float DeltaTime);
};
