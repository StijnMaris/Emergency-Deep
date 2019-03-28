#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzlesGuideBook.generated.h"

class UStaticMeshComponent;

UCLASS()
class EMERGENCYDEEP_API APuzzlesGuideBook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzlesGuideBook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *BookMesh;
	
};
