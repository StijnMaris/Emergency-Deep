

#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/Actor.h"
#include "LightController.generated.h"

UCLASS()
class EMERGENCYDEEP_API ALightController : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ChangeLightState();

	UFUNCTION(BlueprintCallable)
	void SetLightState(bool lightsOn);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LightsOnLevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LightsOffLevelName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool HasLightsOn;

private:
	UFUNCTION(Exec)
	void EDChangeLights();
};
