

#include "LightController.h"
#include "World/LevelLoader.h"

ALightController::ALightController()
	: HasLightsOn{true}
{
	PrimaryActorTick.bCanEverTick = true;
	bNetLoadOnClient = false;
}

void ALightController::BeginPlay()
{
	Super::BeginPlay();

	ALevelLoader::LevelLoader->LoadLevel(LightsOnLevelName);
}

void ALightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightController::ChangeLightState()
{
	HasLightsOn = !HasLightsOn;

	if (HasLightsOn)
	{
		ALevelLoader::LevelLoader->UnLoadLevel(LightsOffLevelName);
		ALevelLoader::LevelLoader->LoadLevel(LightsOnLevelName);
	}
	else
	{
		ALevelLoader::LevelLoader->UnLoadLevel(LightsOnLevelName);
		ALevelLoader::LevelLoader->LoadLevel(LightsOffLevelName);
	}
}

void ALightController::SetLightState(bool lightsOn)
{
	if (lightsOn != HasLightsOn)
	{
		ChangeLightState();
	}
}

void ALightController::EDChangeLights()
{
	ChangeLightState();
}
