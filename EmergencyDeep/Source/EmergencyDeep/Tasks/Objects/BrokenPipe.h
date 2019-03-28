#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EDGrabInterface.h"
#include "BrokenPipe.generated.h"

class UBaseWheel;
class UParticleSystemComponent;
class UBoxComponent;
class UStaticMeshComponent;
class ANewPipe;
class AHammer;
class AOilSpill;

UCLASS()
class EMERGENCYDEEP_API ABrokenPipe : public AActor, public IEDGrabInterface
{
	GENERATED_BODY()
	
public:	
	ABrokenPipe();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere,Replicated)
	UBoxComponent* TriggerBoxFixFar;

	UPROPERTY(VisibleAnywhere,Replicated)
	UStaticMeshComponent* PipePart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PipePosition;

	UPROPERTY(VisibleAnywhere,Replicated)
	UStaticMeshComponent* PipePartBroken;

	UPROPERTY(VisibleAnywhere,Replicated)
	UChildActorComponent* ValvePipe;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	UBoxComponent* TriggerBoxFixClose;

	UPROPERTY(VisibleAnywhere,Replicated)
	UBoxComponent* TriggerBoxLiquid;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Oil;

	// Functions
	UFUNCTION( BlueprintCallable)
	void BrakePipe();

	/*UFUNCTION(reliable, server, WithValidation)
    void Server_BrakePipe();*/

	UFUNCTION( BlueprintCallable)
	void FixPipe();

	UFUNCTION( BlueprintCallable)
	void CheckRequirements();

	UFUNCTION( BlueprintCallable)
	bool IsComplete();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Broken Pipe Sound")
	void OnPipeBreak();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Broken Pipe Sound")
	void OnPipeFix();

	UFUNCTION( BlueprintCallable,BlueprintImplementableEvent, Category = "Broken Pipe Sound")
	void OnHammerHit();

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe")
	TSubclassOf<AOilSpill> OilSpillBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe",Replicated)
	AOilSpill* OilSpill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe",Replicated)
	ANewPipe* NewPipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe",Replicated)
	ANewPipe* NewPipeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe",Replicated)
	ANewPipe* NewPipeRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe",Replicated)
	AHammer* Hammer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe Scale")
	float Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe Scale")
	float ValveScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Broken Pipe Scale")
	float PipeConnectorScale;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe")
	int RequiredHitCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	int HitCountLeft;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	int HitCountRight;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	bool IsPipeFixed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	bool IsOilCleaned;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	bool IsPipeInRightPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	bool IsValveActivated;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe",Replicated)
	bool IsOilOn;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Broken Pipe")
	float BreakDelay;

private:
	// Overlapping
	UFUNCTION()
	void OnBeginOverlapLiquid(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlapLiquid(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void OnBeginOverlapClose(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlapClose(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void OnBeginOverlapFar(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlapFar(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
