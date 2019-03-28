#include "Sonar.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "UObject/UObjectIterator.h"
#include "SonarDetectableObjectComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Widget.h"

// Sets default values
USonar::USonar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	DetectionRadius = 1000.0f;
	SonarRefreshRate = 1.0f;

	RefreshTimer = 0.0f;
}

// Called when the game starts or when spawned
void USonar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USonar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RefreshTimer += DeltaTime;
	if (RefreshTimer >= 1.0f / SonarRefreshRate)
	{
		RefreshTimer -= 1.0f / SonarRefreshRate;
		Refresh();
	}

	PingAlpha = 1.0f - RefreshTimer / 1.0f / SonarRefreshRate;
}

TArray<FSonarObject> USonar::GetCurrentActorsOnSonar()
{
	return CurrentActorsInRangeList;
}

FVector2D USonar::CalculateDotPosition(float circleRadius, const FSonarObject &sonarObject)
{
	float offsetFactor = circleRadius * sonarObject.DistanceFactor;
	FVector2D offset = FVector2D(sonarObject.Direction.X, sonarObject.Direction.Y);
	offset *= offsetFactor;

	return offset;
}

void USonar::FindAllActorsInRange()
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor *actor = *ActorItr;

		TArray<USonarDetectableObjectComponent*> comp;
		actor->GetComponents<USonarDetectableObjectComponent>(comp);

		if (comp.Num() <= 0) continue;
		
		auto otherLocation = comp[0]->GetSonarLocation();
		auto location = GetOwner()->GetActorLocation();
		auto vecToOther = otherLocation - location;
		
		auto distance = vecToOther.Size();
		if (distance <= DetectionRadius)
		{
			FSonarObject foundObject = FSonarObject();

			foundObject.Actor = actor;
			foundObject.DistanceFactor = distance / DetectionRadius;
			vecToOther.Normalize();
			foundObject.Direction = vecToOther;
			CurrentActorsInRangeList.Add(foundObject);
		}
	}
}

void USonar::Refresh()
{
	CurrentActorsInRangeList.Empty();

	FindAllActorsInRange();

	OnRefresh_BP();
}
