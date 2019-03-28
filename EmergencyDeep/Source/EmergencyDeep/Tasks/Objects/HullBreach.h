

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HullBreach.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;
class ACork;

UCLASS()
class EMERGENCYDEEP_API AHullBreach : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHullBreach();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Damage();

	UFUNCTION(BlueprintCallable)
	bool IsLeaking();
	   
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* HullBreachMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UParticleSystemComponent* LeakParticleEmitter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* AttackTrigger;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* CorkTrigger;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool Broken = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool Corked = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ACork* Cork;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnBreached"))
		void OnBreached_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUnCorked"))
		void OnUnCorked_BP();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnCorked"))
		void OnCorked_BP();
private:
	UFUNCTION()
		void OnBeginOverlapAttack(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void OnBeginOverlapCork(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);


	UFUNCTION()
	void CloseBreach(ACork* actor);
	
};
