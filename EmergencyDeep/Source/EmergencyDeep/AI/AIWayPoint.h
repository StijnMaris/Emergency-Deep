

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIWayPoint.generated.h"

class UBillboardComponent;

UCLASS()
class EMERGENCYDEEP_API AAIWayPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIWayPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBillboardComponent *SpriteComponent;
	
};
