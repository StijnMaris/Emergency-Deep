#include "AIWayPoint.h"
#include "Components/BillboardComponent.h"

// Sets default values
AAIWayPoint::AAIWayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (SpriteComponent)
	{
		RootComponent = SpriteComponent;
	}
	else
	{
		SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
		//SpriteComponent->SetVisibility(false);
		RootComponent = SpriteComponent;
	}
}

// Called when the game starts or when spawned
void AAIWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

