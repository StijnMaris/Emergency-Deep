#include "PuzzlesGuideBook.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APuzzlesGuideBook::APuzzlesGuideBook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BookMesh"));
	RootComponent = BookMesh;
}

// Called when the game starts or when spawned
void APuzzlesGuideBook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzlesGuideBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

