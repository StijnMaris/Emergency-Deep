

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WireConnection.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class EMERGENCYDEEP_API AWireConnection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWireConnection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* ConnectionPin;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	bool IsConnected = false;

	UPROPERTY(EditAnywhere, Replicated)
	int Id;

	UPROPERTY(EditAnywhere)
	int RowId;
};
