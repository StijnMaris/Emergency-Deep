

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmergencyLight.generated.h"

UCLASS()
class EMERGENCYDEEP_API AEmergencyLight : public AActor
{
	GENERATED_BODY()
	
public:	
	AEmergencyLight();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


};
