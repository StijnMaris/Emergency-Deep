#include "AIDamageComponent.h"

// Sets default values for this component's properties
UAIDamageComponent::UAIDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	Damage = 0.0f;
}

// Called when the game starts
void UAIDamageComponent::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void UAIDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAIDamageComponent::DealDamage(float damage)
{
	if (damage <= 0.0f) return;

	float oldDamage = Damage;
	Damage += damage;

	if (Damage >= MaxHealth)
	{
		Damage = MaxHealth;
		bDeath = true;
		OnDeath_BP();
	}

	if (oldDamage != Damage)
		OnDamage_BP(damage);
}
