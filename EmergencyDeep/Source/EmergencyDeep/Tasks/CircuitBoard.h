

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/BaseInteractable.h"
#include "CircuitBoard.generated.h"

class UGrippableStaticMeshComponent;
class UBoxComponent;
class UPointLightComponent;
class AWire;
class ABaseLever;
class UCurveFloat;

UCLASS()
class EMERGENCYDEEP_API ACircuitBoard : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	

	ACircuitBoard();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

	UFUNCTION(BlueprintCallable)
	void CheckConnections();

	UFUNCTION(BlueprintCallable)
	void KickStart();

	UFUNCTION(BlueprintCallable)
	void BreakFuseBox(bool kickStartOnly);

	UFUNCTION(BlueprintCallable)
	void AutoSolve();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* FuseBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* LightMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Light")
	UPointLightComponent* Light;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* Door;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* DoorClosedTrigger;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* ConstraintPositioner;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UPhysicsConstraintComponent* DoorConstraint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<AWire*> Wires;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWire> ToSpawnWire;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UChildActorComponent* Lever;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootWires;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> WirePlacements;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FLinearColor> CollorLegend;

	UPROPERTY(EditAnywhere)
	TArray<ACircuitBoard*> LinkedCirquits;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int NrWireCouples;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DelayKickstart = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DoorMaxAngle = 90.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool Fixed = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool KickStarted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light")
	UCurveFloat* LightKickStart;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light")
	UCurveFloat* LightShutDown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light")
	float MaxLightKickStartTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light")
	float MaxLightShutDownTime;


protected:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSolved"))
		void OnSolved_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnNotSolved"))
		void OnNotSolved_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnClosePanel"))
		void OnClosePanel_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnOpenPanel"))
		void OnOpenPanel_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnBreakFusebox"))
		void OnBreakFusebox_BP(bool BrokenWires);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnKickStarted"))
		void OnKickStarted_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnKickStartFail"))
		void OnKickStartFail_BP();

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void SpawnWires();

	UFUNCTION()
	TArray<int> CheckLinks(int wireI);

	UFUNCTION()
	void RelinkWire(int wireId, int& failedLink);

	UFUNCTION()
	void UpdateLight(float deltaTime);

	UPROPERTY(VisibleAnywhere)
	float CurrentTime = 0.0f;

	UPROPERTY()
	float LightBrightness;

	UPROPERTY()
	float CurrentLightTimeStart;

	UPROPERTY()
	float CurrentLightTimeShut;


	
};
