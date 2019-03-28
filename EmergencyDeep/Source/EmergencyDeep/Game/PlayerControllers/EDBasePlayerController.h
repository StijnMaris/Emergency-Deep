

#pragma once

#include "EmergencyDeep.h"
#include "GameFramework/PlayerController.h"
#include "EDBasePlayerController.generated.h"

class APortableRadio;
class AEDPlayerCharacter;
class AEDPlayerState;

/**
 *
 */
UCLASS()
class EMERGENCYDEEP_API AEDBasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AEDBasePlayerController();

	void BeginPlay() override;
	void SetupInputComponent() override;

	void StartTeleportLarge();
	void StartTeleportShort();
	void EndTeleport();

	void MoveForward(float throttle);
	void MoveRight(float throttle);
	void Crouch();
	void Yaw(float throttle);

	void Pitch(float throttle);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Pitch(float throttle);

	void GrabLeft();
	void GrabRight();
	void SideGripLeft();
	void SideGripRight();
	void DropLeft();
	void DropRight();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShootLeft();
	void ServerShootLeft_Implementation();
	bool ServerShootLeft_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShootRight();
	void ServerShootRight_Implementation();
	bool ServerShootRight_Validate();

protected:
	AEDPlayerCharacter* GetEDCharacter() const;
	AEDPlayerState* GetEDPlayerState() const;
};
