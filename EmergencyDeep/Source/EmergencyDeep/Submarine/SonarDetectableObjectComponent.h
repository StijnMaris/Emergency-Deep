#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SonarDetectableObjectComponent.generated.h"

UENUM(BlueprintType)
enum class ESonarObjectType : uint8
{
	SOT_Neutral UMETA(DisplayName = "Neatral"),
	SOT_Hostile UMETA(DisplayName = "Hostile"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERGENCYDEEP_API USonarDetectableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USonarDetectableObjectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetSonarLocation();
	FVector GetSonarLocation_Implementation();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FColor SonarPingColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bOverrideLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector OverrideLocation;
};
