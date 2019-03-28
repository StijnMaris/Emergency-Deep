

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/BaseInteractable.h"
#include "Wire.generated.h"

class UGrippableStaticMeshComponent;
class UBoxComponent;
class UCableComponent;
class UMaterialInterface;


UCLASS()
class EMERGENCYDEEP_API AWire : public AActor, public IEDGrabInterface
{

	GENERATED_BODY()
	
public:	
	AWire();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Root;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* CableGrab;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* WireTrigger;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UPhysicsConstraintComponent* Constraint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCableComponent* Cable;

	// Variables
	UPROPERTY(EditAnywhere)
	int Id;

	UPROPERTY(EditAnywhere)
	bool IsConnected;

	UPROPERTY(EditAnywhere, Category = "Cable")
	float WireLength = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Cable")
	float WireWidth = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Cable")
	float MaxWireLength = 100.0f;

	UPROPERTY(EditAnywhere)
	bool IsConnector = false;

	// Functions
	UFUNCTION(BlueprintCallable)
	void Connect(AWire* otherWire);

	UFUNCTION(BlueprintCallable)
	void DisConnect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable)
	AWire* GetConnectedWire();

	UFUNCTION(BlueprintCallable)
	AWire* GetLastOverlappingWire();

	UFUNCTION(BlueprintCallable)
	void SetLastOverlappingWire(AWire* otherWire);

	UFUNCTION(BlueprintCallable)
	bool ValidateConnection();

	UFUNCTION(BlueprintCallable)
	bool CheckDistance();

	UFUNCTION(BlueprintCallable)
	void SetColor(FLinearColor col);

protected:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnConnect"))
		void OnConnect_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDisconnect"))
		void OnDisconnect_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMaxLengthSnapped"))
		void OnMaxLengthSnapped_BP();

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	AWire* ConnectedWire;

	AWire* LastOverlappingWire;
	
	UPROPERTY()
	USceneComponent* MotionContr;

	UPROPERTY()
	UMaterialInstanceDynamic* WireMaterialInstance;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* WireMaterialBase;
	
	UPROPERTY(EditAnywhere)
	FLinearColor Color;


};
