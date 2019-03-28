#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIDamageComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UAIDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void DealDamage(float damage);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDamage"))
	void OnDamage_BP(float damage);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeath"))
	void OnDeath_BP();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Damage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bDeath;
};
