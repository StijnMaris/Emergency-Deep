#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/Actor.h"
#include "CaveSegmentController.h"
#include "WorldGenerator.generated.h"

UCLASS()
class EMERGENCYDEEP_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldGenerator();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float CurrentDepth;

	UPROPERTY(EditAnywhere)
	FName StartLevelToLoad;
	
	UPROPERTY(EditAnywhere)
	float LowDepth;

	UPROPERTY(EditAnywhere)
	TArray<FName> LowDepthEnvironmentLevels;

	UPROPERTY(EditAnywhere)
	float MediumDepth;
	UPROPERTY(EditAnywhere)
	float MediumDepthEnvironmentChance;

	UPROPERTY(EditAnywhere, Category = "World Rotation")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category="World Rotation")
	float DeltaRotationScale;

	UPROPERTY(EditAnywhere, Category = "World Rotation")
	float RotationTresshold;
	
	UPROPERTY(EditAnywhere)
	TArray<FName> MediumDepthEnvironmentLevels;
	
	UPROPERTY(EditAnywhere)
	float HighDepth;
	UPROPERTY(EditAnywhere)
	float HighDepthEnvironmentChance;

	UPROPERTY(EditAnywhere)
	TArray<FName> HighDepthEnvironmentLevels;

	TArray<UCaveSegmentController*> ActiveCaveSegments;

	static AWorldGenerator *ActiveWorldGenerator;
	FName CurrentLoadedLevelName;
	FName PreviouslyLoadedLevelName;

	void LoadNext();
	void UnloadLast();
	void RegisterNewCaveSegment(UCaveSegmentController *Segment);
	void OnDestroyCaveSegment(UCaveSegmentController *Segment);

	UFUNCTION(BlueprintCallable)
	void MoveWorld(float Speed);

	UFUNCTION(BlueprintCallable)
	void RotateWorld(float Rotation);

	UFUNCTION(BlueprintCallable)
	float GetTotalDelta();

	UFUNCTION(BlueprintCallable)
	float GetDesiredRotation();

	UCaveSegmentController *GetLastSegmentController();

private:
	const FName SelectSegmentToSpawn() const;
	const FName PickLowDepthEnvironment() const;
	const FName PickMediumDepthEnvironment() const;
	const FName PickHighDepthEnvironment() const;

	void UpdateWorldRotations();

	float CurrentWorldRotation;
	float DesiredWorldRotation;

	float TotalRotateDelta;
};
