

#pragma once

#include "EmergencyDeep.h"
#include "Interactables/BaseInteractable.h"
#include "Phone.generated.h"

class UCableComponent;
class APhoneHorn;

UCLASS()
class EMERGENCYDEEP_API APhone : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	APhone();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* PhoneBaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* PhoneBaseCablePosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCableComponent* PhoneCable;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UChildActorComponent* Horn;

private:
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandleComponent;

	/**
	 * \brief Check if the player is too far with the horn then detach the horn from the player hand.
	 */
	void CheckDetachCable();
};
