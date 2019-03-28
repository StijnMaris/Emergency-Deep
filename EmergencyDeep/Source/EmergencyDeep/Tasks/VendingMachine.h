

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "VendingMachine.generated.h"

class UStaticMeshComponent;
class ABasePullButton;
class UGrippableStaticMeshComponent;
class UPhysicsConstraintComponent;
class UBoxComponent;
class APrintables;
class AActor;

UCLASS()
class EMERGENCYDEEP_API AVendingMachine : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVendingMachine();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Properties
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ABasePullButton*> PullButtons;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABasePullButton> ToSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APrintables> PrintableSpawn;

	UPROPERTY(EditAnywhere)
	TArray<APrintables*> PrintableTemplates;

	UPROPERTY(BlueprintReadOnly)
	TArray<int> ActiveButtons;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UGrippableStaticMeshComponent* LootCabin;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* VendingMachineMesh;

	UPROPERTY(EditAnywhere)
	FVector SpawnPrintOffset;

	// Functions
	UFUNCTION(BlueprintCallable)
	void UpdateActiveButtons();

	UFUNCTION(BlueprintCallable)
	void ResetButtons();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrab(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrabRelease(UPhysicsHandleComponent* PhysicsHandle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	USceneComponent* ConstraintPositioner;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* Constraint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootButtons;

	UPROPERTY(EditAnywhere)
	FVector2D ButtonsPlacement;

	UPROPERTY(EditAnywhere, Category = "PullButton")
	float LinearLimit;

	UPROPERTY(EditAnywhere, Category = "PullButton")
	float MotorForce;

	UPROPERTY(VisibleAnywhere)
	bool IsPulled;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
	   
	void CreateButtons();
	int CheckPrints();
	void SetConstraint();
};
