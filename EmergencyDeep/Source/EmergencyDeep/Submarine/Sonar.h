#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Sonar.generated.h"

class UWidgetComponent;
class UUserWidget;

USTRUCT(BlueprintType)
struct FSonarObject
{
	GENERATED_BODY()

	FSonarObject() : Actor(nullptr), DistanceFactor(0.0f), Direction(FVector(0.0f, 0.0f, 0.0f)) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sonar Object")
	AActor *Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sonar Object")
	float DistanceFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sonar Object")
	FVector Direction;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMERGENCYDEEP_API USonar : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USonar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	TArray<FSonarObject> GetCurrentActorsOnSonar();

	UFUNCTION(BlueprintCallable)
	FVector2D CalculateDotPosition(float circleRadius, const FSonarObject &sonarObject);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSonarRefresh"))
	void OnRefresh_BP();

public:
	UPROPERTY(EditAnywhere)
	float DetectionRadius;

	UPROPERTY(EditAnywhere)
	float SonarRefreshRate;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float PingAlpha;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UserWidget;

private:
	void FindAllActorsInRange();
	void Refresh();

private:
	TArray<FSonarObject> CurrentActorsInRangeList;
	float RefreshTimer;
};
