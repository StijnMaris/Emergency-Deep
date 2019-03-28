

#pragma once

#include "EmergencyDeep.h"
#include "BaseInteractable.h"
#include "BaseButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonReleased);

UCLASS()
class EMERGENCYDEEP_API ABaseButton : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	ABaseButton();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnButtonPressed();
	virtual void OnButtonReleased();

	UPROPERTY(BlueprintAssignable)
	FButtonPressed OnButtonPressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FButtonReleased OnButtonReleasedDelegate;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* ButtonBaseMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* ButtonTrigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Button")
	float LinearLimit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Button")
	float Stiffness;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Button")
	float PressedAmount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Button")
	bool IsPressed;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnButtonPressed"))
	void OnButtonPressed_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnButtonReleased"))
	void OnButtonReleased_BP();

private:
	UFUNCTION()
	void OnBeginOverlapTrigger(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlapTrigger(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void OnBeginOverlapMesh(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlapMesh(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UPROPERTY()
	UPrimitiveComponent* OverlappedComponent{};

	FVector InitialOverlapLocation;
	FVector InitialButtonLocation;
};
