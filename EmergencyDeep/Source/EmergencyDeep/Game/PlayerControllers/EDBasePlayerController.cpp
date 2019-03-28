

#include "EDBasePlayerController.h"
#include "Game/PlayerControllers/EDGamePlayerController.h"
#include "Player/EDPlayerCharacter.h"
#include "Player/EDPlayerHand.h"
#include "Game/PlayerStates/EDPlayerState.h"
#include "Game/EDCheatManager.h"
#include "Interactables/HarpoonGun.h"

AEDBasePlayerController::AEDBasePlayerController()
{
	CheatClass = UEDCheatManager::StaticClass();
}

void AEDBasePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AEDBasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Grabbing
	InputComponent->BindAction("GrabRight", IE_Pressed, this, &AEDBasePlayerController::GrabRight);
	InputComponent->BindAction("GrabLeft", IE_Pressed, this, &AEDBasePlayerController::GrabLeft);
	InputComponent->BindAction("GrabRight", IE_Released, this, &AEDBasePlayerController::DropRight);
	InputComponent->BindAction("GrabLeft", IE_Released, this, &AEDBasePlayerController::DropLeft);

	InputComponent->BindAction("SideGripRight", IE_Pressed, this, &AEDBasePlayerController::SideGripRight);
	InputComponent->BindAction("SideGripLeft", IE_Pressed, this, &AEDBasePlayerController::SideGripLeft);

	// Movement
	InputComponent->BindAxis("Forward", this, &AEDBasePlayerController::MoveForward);
	InputComponent->BindAxis("Right", this, &AEDBasePlayerController::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AEDBasePlayerController::Pitch);
	InputComponent->BindAxis("Turn", this, &AEDBasePlayerController::Yaw);

	InputComponent->BindAction("Crouch",IE_Pressed, this, &AEDBasePlayerController::Crouch);

	// Teleport
	InputComponent->BindAction("TeleportLarge", IE_Pressed, this, &AEDBasePlayerController::StartTeleportLarge);
	InputComponent->BindAction("TeleportLarge", IE_Released, this, &AEDBasePlayerController::EndTeleport);
	InputComponent->BindAction("TeleportSmall", IE_Pressed, this, &AEDBasePlayerController::StartTeleportShort);
	InputComponent->BindAction("TeleportSmall", IE_Released, this, &AEDBasePlayerController::EndTeleport);

	// Harpoon shooting
	InputComponent->BindAction("FireWeaponLeft", IE_Pressed, this, &AEDBasePlayerController::ServerShootLeft);
	InputComponent->BindAction("FireWeaponRight", IE_Pressed, this, &AEDBasePlayerController::ServerShootRight);
}

void AEDBasePlayerController::StartTeleportLarge()
{
	if (GetEDCharacter())
	{
		GetEDCharacter()->RightHand->ActivateTeleport(true);
	}
}

void AEDBasePlayerController::StartTeleportShort()
{
	if (GetEDCharacter())
	{
		GetEDCharacter()->RightHand->ActivateTeleport(false);
	}
}

void AEDBasePlayerController::EndTeleport()
{
	if (GetEDCharacter())
	{
		GetEDCharacter()->RightHand->DeActivateTeleport();
	}
}

void AEDBasePlayerController::MoveForward(float throttle)
{
	if(GetEDCharacter() && throttle != 0.0f)
	{
		GetEDCharacter()->MoveForward(throttle);
	}
}

void AEDBasePlayerController::MoveRight(float throttle)
{
	if (GetEDCharacter() && throttle != 0.0f)
	{
		GetEDCharacter()->MoveRight(throttle);
	}
}

void AEDBasePlayerController::Crouch()
{
	if(GetEDCharacter())
	{
		GetEDCharacter()->Crouch(true);
	}
}

void AEDBasePlayerController::Pitch(float throttle)
{
	if (GetEDCharacter() && throttle != 0.0f)
	{
		if (!HasAuthority())
		{
			Server_Pitch(throttle);
		}

		const FRotator rotator(-throttle, 0, 0);
		GetEDCharacter()->VrCamera->AddRelativeRotation(rotator);
	}
}

void AEDBasePlayerController::Server_Pitch_Implementation(float throttle)
{
	Pitch(throttle);
}

bool AEDBasePlayerController::Server_Pitch_Validate(float throttle)
{
	return true;
}

void AEDBasePlayerController::Yaw(float throttle)
{
	AddYawInput(throttle);
}

void AEDBasePlayerController::GrabLeft()
{
	if (GetEDCharacter() && GetEDCharacter()->LeftHand)
	{
		GetEDCharacter()->LeftHand->GrabObject();
	}
}

void AEDBasePlayerController::GrabRight()
{
	if (GetEDCharacter() && GetEDCharacter()->RightHand)
	{
		GetEDCharacter()->RightHand->GrabObject();
	}
}

void AEDBasePlayerController::SideGripLeft()
{
	if (GetEDCharacter() && GetEDCharacter()->LeftHand)
	{
		GetEDCharacter()->LeftHand->SideGripObject();
	}
}

void AEDBasePlayerController::SideGripRight()
{
	if (GetEDCharacter() && GetEDCharacter()->RightHand)
	{
		GetEDCharacter()->RightHand->SideGripObject();
	}
}

void AEDBasePlayerController::DropLeft()
{
	if (GetEDCharacter() && GetEDCharacter()->LeftHand)
	{
		GetEDCharacter()->LeftHand->DropObject();
	}
}

void AEDBasePlayerController::DropRight()
{
	if (GetEDCharacter() && GetEDCharacter()->RightHand)
	{
		GetEDCharacter()->RightHand->DropObject();
	}
}

void AEDBasePlayerController::ServerShootLeft_Implementation()
{
	auto leftGrabbedObj = GetEDCharacter()->LeftHand->GetAttachedActor();
	if (!leftGrabbedObj) return;
	auto leftHandHarpoon = Cast<AHarpoonGun>(leftGrabbedObj);

	if (leftHandHarpoon)
	{
		leftHandHarpoon->TryShoot();
	}
}

bool AEDBasePlayerController::ServerShootLeft_Validate()
{
	return true;
}

void AEDBasePlayerController::ServerShootRight_Implementation()
{
	auto rightGrabbedObj = GetEDCharacter()->RightHand->GetAttachedActor();
	if (!rightGrabbedObj) return;
	auto rightHandHarpoon = Cast<AHarpoonGun>(rightGrabbedObj);

	if (rightHandHarpoon)
	{
		rightHandHarpoon->TryShoot();
	}
}

bool AEDBasePlayerController::ServerShootRight_Validate()
{
	return true;
}

AEDPlayerCharacter* AEDBasePlayerController::GetEDCharacter() const
{
	AEDPlayerCharacter* character = Cast<AEDPlayerCharacter>(GetPawn());
	return character;
}

AEDPlayerState* AEDBasePlayerController::GetEDPlayerState() const
{
	AEDPlayerState* state = Cast<AEDPlayerState>(PlayerState);
	return state;
}