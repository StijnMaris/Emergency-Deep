#include "SliderPosition.h"
#include "Components/BoxComponent.h"
#include "BaseSlider.h"

// Sets default values for this component's properties
ASliderPosition::ASliderPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASliderPosition::OnBeginOverlap);

	NumberOfSteps = 5;
}

// Called when the game starts
void ASliderPosition::BeginPlay()
{
	Super::BeginPlay();

	OwnerSlider = Cast<ABaseSlider>(GetParentActor());
	if (OwnerSlider) OwnerTrigger = OwnerSlider->HandleTriggerBox;
}

// Called every frame
void ASliderPosition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASliderPosition::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(otherActor == OwnerSlider && otherComp == OwnerTrigger)
		if(OwnerSlider)
			OwnerSlider->ChangeSliderValue(SliderPositionValue / static_cast<float>(NumberOfSteps));
}