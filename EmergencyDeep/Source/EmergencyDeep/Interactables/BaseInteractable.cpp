

#include "BaseInteractable.h"

ABaseInteractable::ABaseInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

