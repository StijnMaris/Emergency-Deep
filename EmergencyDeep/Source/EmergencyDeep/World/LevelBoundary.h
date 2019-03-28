#pragma once

#include "EmergencyDeep.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBoundary.generated.h"

UENUM(BlueprintType)
enum class ELevelBoundaryType : uint8
{
	BT_LoadNew UMETA(DisplayName="Load New"),
	BT_Unload UMETA(DisplayName="Unload Self"),
};

UCLASS()
class EMERGENCYDEEP_API ALevelBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelBoundary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OverlapBegins(UPrimitiveComponent *OverlapComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAcces = "true"))
	UBoxComponent *OverlapVolume;

	UPROPERTY(EditAnywhere)
	ELevelBoundaryType BoundaryType;

private:
	bool bDidGeneration = false;
};
