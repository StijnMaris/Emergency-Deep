

#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "BaseInteractable.generated.h"

UCLASS()
class EMERGENCYDEEP_API ABaseInteractable : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	ABaseInteractable();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	
};
