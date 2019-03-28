

#include "EmergencyLight.h"


// Sets default values
AEmergencyLight::AEmergencyLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEmergencyLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEmergencyLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

