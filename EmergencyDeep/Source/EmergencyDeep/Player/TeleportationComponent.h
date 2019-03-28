

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TeleportationComponent.generated.h"

class USplineMeshComponent;
class USplineComponent;
class UArrowComponent;
class UMaterialInterface;
class AEDPlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API UTeleportationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeleportationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetupComponent(UArrowComponent* arcDirection, USplineComponent* spline, UStaticMeshComponent* destinationMarker, AEDPlayerCharacter* character);

	UFUNCTION()
	void UpdateDestinationMarker();

	UFUNCTION()
	void BeginTeleport();

	UFUNCTION()
	bool CanTeleport() const;

	UFUNCTION()
	bool IsTeleporting() const;

	UFUNCTION()
	void ResetDestination();

private:

	// Components

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArcDirection;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* ArcSpline;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TeleportLocationMarker;

	UPROPERTY()
	TArray<USplineMeshComponent*> DynamicMeshes;
	
	// Variables

	UPROPERTY()
	AEDPlayerCharacter* Character;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ArcMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* ArcMaterial;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 1.0f;

	UPROPERTY(VisibleAnywhere)
	FVector TeleportProjection = FVector(500, 500, 500);

	UPROPERTY(VisibleAnywhere)
	bool bIsTeleporting = false;

	UPROPERTY()
	bool bCanTeleport = false;

	UPROPERTY()
	bool bIsInit = false;

	UPROPERTY(EditAnywhere)
	float TeleportProjectileSpeed = 1000;

	UPROPERTY()
	FVector TeleportationDestination;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	// Functions


	void FinishTeleport();
	void UpdateSpline(const TArray<FVector> &path);

};
