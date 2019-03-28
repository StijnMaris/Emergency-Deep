

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HullBreachHolderComponent.generated.h"

class AHullBreach;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UHullBreachHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHullBreachHolderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TArray<AHullBreach*> HullBreaches;
};
