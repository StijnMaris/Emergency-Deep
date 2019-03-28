#include "CaveFoliage.h"
#include "WorldGenerator.h"

UCaveFoliage::UCaveFoliage()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCaveFoliage::BeginPlay()
{
	Super::BeginPlay();

	if (AWorldGenerator::ActiveWorldGenerator)
	{
		auto segmentController = AWorldGenerator::ActiveWorldGenerator->GetLastSegmentController();
		if (segmentController)
			segmentController->AddFoliage(this);
	}
}

void UCaveFoliage::Move_Implementation(float Rotation)
{
}

void UCaveFoliage::UpdateRotation_Implementation(float Rotation)
{
}
