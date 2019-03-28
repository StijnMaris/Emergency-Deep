// Fill out your copyright notice in the Description page of Project Settings.

#include "EDPlayerCharacter.h"
#include "EDPlayerHand.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Online/Voice/PlayerVoiceComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionControllerComponent.h"
#include "Player/CharacterAnimInstance.h"
#include "Game/PlayerControllers/EDBaseNetworkedPlayerController.h"
#include "MotionControllerComponent.h"

AEDPlayerCharacter::AEDPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	VrOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VrOrigin"));
	VrCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VrCamera"));
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PlayerVoiceComponent = CreateDefaultSubobject<UPlayerVoiceComponent>(TEXT("PlayerVoice"));
	PlayerNameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerNameComponent"));
	LeftHandMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHandMotionController"));
	RightHandMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHandMotionController"));

	VrOrigin->SetupAttachment(GetCapsuleComponent());
	VrCamera->SetupAttachment(VrOrigin);
	PostProcessComponent->SetupAttachment(GetRootComponent());
	PlayerVoiceComponent->SetupAttachment(RootComponent);
	PlayerNameComponent->SetupAttachment(RootComponent);
	LeftHandMotionController->SetupAttachment(VrOrigin);
	RightHandMotionController->SetupAttachment(VrOrigin);

	UseBlinker = true;

	HeadHeight = 25.0f;

	PlayerNameComponent->SetOwnerNoSee(true);

	HasInitMotionControllers = false;

	LeftHandMotionController->PlayerIndex = -1;
	RightHandMotionController->PlayerIndex = -1;
	LeftHandMotionController->MotionSource = FName{ "" };
	RightHandMotionController->MotionSource = FName{ "" };

	VrOrigin->SetIsReplicated(true);
	VrCamera->SetIsReplicated(true);
	GetMesh()->SetIsReplicated(true);
}

void AEDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEDPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AEDBaseNetworkedPlayerController* pc = Cast< AEDBaseNetworkedPlayerController>(GetController());
	if(pc)
	{
		pc->DestroyHands(LeftHand, RightHand);
	}

	Super::EndPlay(EndPlayReason);
}

void AEDPlayerCharacter::SpawnMotionControllers()
{
	const FTransform transform{};
	const FAttachmentTransformRules attachmentRules{ EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false };

	// Left hand spawn
	LeftHand = Cast<AEDPlayerHand>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, PlayerHand, transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
	LeftHand->Initialize(EControllerHand::Left);
	LeftHand->FinishSpawning(transform, true);
	LeftHand->AttachToComponent(VrOrigin, attachmentRules);

	// Right hand spawn
	RightHand = Cast<AEDPlayerHand>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, PlayerHand, transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
	RightHand->Initialize(EControllerHand::Right);
	RightHand->FinishSpawning(transform, true);
	RightHand->AttachToComponent(VrOrigin, attachmentRules);

	OnRep_HandsReplicated();
}

void AEDPlayerCharacter::SetUpPlayerHeight()
{
	FName hMDName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();
	ED_LOG(LogED_Game, Log, TEXT("AEDPlayerCharacter::SetUpPlayerHeight: HMD: %s"), *hMDName.ToString());

	if (hMDName.IsEqual(FName{ "SteamVR" }) && !OverrideFpsMode || OverrideFpsMode && !IsFpsMode)
	{
		IsFpsMode = false;

		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
	else if (hMDName.IsEqual(FName{ "None" }) && !OverrideFpsMode || OverrideFpsMode && IsFpsMode)
	{
		IsFpsMode = true;

		bUseControllerRotationYaw = true;

		// Put Camera and hands on eye height
		VrCamera->SetRelativeLocation(FVector{ 0, 0, 120 });
		LeftHandMotionController->SetRelativeLocation(FVector{ 50, -30 ,0 });
		RightHandMotionController->SetRelativeLocation(FVector{ 50 ,30 ,0 });

		// Attach Hands to Camera for better control
		const FAttachmentTransformRules attachmentRules{ EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false };
		LeftHandMotionController->AttachToComponent(VrCamera, attachmentRules);
		RightHandMotionController->AttachToComponent(VrCamera, attachmentRules);
	}
}

void AEDPlayerCharacter::Crouch(bool bClientSimulation)
{
	VrCamera->SetRelativeLocation(FVector{ VrCamera->RelativeLocation.X,VrCamera->RelativeLocation.Y,VrCamera->RelativeLocation.Z - 20 });
}

void AEDPlayerCharacter::SetupBlinker()
{
	if (BlinkMaterialBase != nullptr)
	{
		BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkMaterialBase, this);
		PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
	}
}

void AEDPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBlinkers();
	UpdateVrOrigin();
	UpdateVRRotations();	
	UpdateMotionControllers();
	UpdateAnimations();
}

void AEDPlayerCharacter::UpdateBlinkers() const
{
	if (RadiusVsVelocity != nullptr && UseBlinker)
	{
		float speed = GetVelocity().Size();
		float radius = 10.0f;
		FVector2D center = FVector2D(0.5f,0.5f);
		if(IsFpsMode == false)
		{
			radius = RadiusVsVelocity->GetFloatValue(speed);
			center = GetBlinkerCenter();
		}
		UE_LOG(LogTemp, Warning, TEXT("Radius: %f, Center: %s"), radius, *center.ToString());
		BlinkerMaterialInstance->SetScalarParameterValue(TEXT("Radius"), radius);

		BlinkerMaterialInstance->SetVectorParameterValue(TEXT("Center"), FLinearColor(center.X, center.Y, 0));
	}
}

void AEDPlayerCharacter::UpdateVrOrigin()
{
	if(Helpers::IsLocalPlayer(this, NetOwnerId))
	{
		FVector cameraOffset = VrCamera->GetComponentLocation() - GetActorLocation();
		cameraOffset.Z = 0;
		AddActorWorldOffset(cameraOffset);
		VrOrigin->AddWorldOffset(-cameraOffset);
		UpdatePlayerCapsule();
		Server_UpdateVROrigin(VrOrigin->RelativeLocation);
	}
}

void AEDPlayerCharacter::UpdatePlayerCapsule() const
{
	if (!IsFpsMode)
	{
		float height = VrCamera->GetComponentLocation().Z - VrOrigin->GetComponentLocation().Z + HeadHeight;
		GetCapsuleComponent()->SetCapsuleHalfHeight(height/2.0f);
		FVector pos = VrOrigin->GetComponentLocation();
		VrOrigin->SetWorldLocation(FVector(pos.X, pos.Y, GetCapsuleComponent()->GetComponentLocation().Z - height / 2.0f));
	}
}

void AEDPlayerCharacter::UpdateVRRotations()
{
	if (Helpers::IsLocalPlayer(this, NetOwnerId))
	{
		const FVector cameraPosition = VrCamera->GetComponentLocation();
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(cameraPosition, cameraPosition + VrCamera->GetForwardVector());
		rotation.Pitch = 0.0f;
		rotation.Roll = 0.0f;
		rotation.Yaw += -90.0f;

		SetMeshRotation(rotation);
	}
}

void AEDPlayerCharacter::UpdateMotionControllers()
{
	if (Helpers::IsLocalPlayer(this, NetOwnerId))
	{
		if (!IsFpsMode && !HasInitMotionControllers)
		{
			LeftHandMotionController->SetAssociatedPlayerIndex(0);
			RightHandMotionController->SetAssociatedPlayerIndex(0);
			LeftHandMotionController->SetTrackingMotionSource(FName{ "Left" });
			RightHandMotionController->SetTrackingMotionSource(FName{ "Right" });

			HasInitMotionControllers = true;
		}

		FMotionControllerReplicationData leftHand{};
		leftHand.Set(LeftHandMotionController->GetComponentLocation(), LeftHandMotionController->GetComponentRotation());
		FMotionControllerReplicationData rightHand{};
		rightHand.Set(RightHandMotionController->GetComponentLocation(), RightHandMotionController->GetComponentRotation());

		Server_UpdateMotionControllers(leftHand, rightHand);
		LeftHand->SetActorTransform(LeftHandMotionController->GetComponentTransform());
		RightHand->SetActorTransform(RightHandMotionController->GetComponentTransform());
	}
}

void AEDPlayerCharacter::UpdateAnimations() const
{
	if (LeftHand && RightHand)
	{
		UCharacterAnimInstance* animInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		check(animInstance)
		animInstance->SetHandStates(LeftHand->GetGrabState(), RightHand->GetGrabState());
	}
}

void AEDPlayerCharacter::Server_UpdateVROrigin_Implementation(const FVector_NetQuantize10& vrOriginLocation)
{
	VrOrigin->RelativeLocation = vrOriginLocation;
}

ED_DEFAULT_SERVER_VALIDATION(AEDPlayerCharacter, UpdateVROrigin, const FVector_NetQuantize10& vrOriginLocation)

void AEDPlayerCharacter::Server_UpdateMeshRotation_Implementation(const FRotator& rotation)
{
	SetMeshRotation(rotation);
}

ED_DEFAULT_SERVER_VALIDATION(AEDPlayerCharacter, UpdateMeshRotation, const FRotator& rotation)

void AEDPlayerCharacter::SetMeshRotation(const FRotator& rotation)
{
	FRotator* a = &GetMesh()->RelativeRotation;

	if (!HasAuthority())
	{
		Server_UpdateMeshRotation(rotation);
	}
	else
	{
		GetMesh()->SetWorldRotation(rotation);
	}
}

FVector2D AEDPlayerCharacter::GetBlinkerCenter() const
{
	FVector movementDir = GetVelocity().GetSafeNormal();
	if (movementDir.IsNearlyZero())
	{
		return FVector2D(0.5f, 0.5f);
	}

	const FVector tunnelWorldLocation = VrCamera->GetComponentLocation() + movementDir * 100;

	APlayerController* player = Cast<APlayerController>(GetController());
	if (player == nullptr)
	{
		return FVector2D(0.5f, 0.5f);
	}

	FVector2D tunnelScreenLocation;
	player->ProjectWorldLocationToScreen(tunnelWorldLocation, tunnelScreenLocation);

	int32 sizeX, sizeY;
	player->GetViewportSize(sizeX, sizeY);
	tunnelScreenLocation.X /= sizeX;
	tunnelScreenLocation.Y /= sizeY;

	return tunnelScreenLocation;
}

void AEDPlayerCharacter::MoveForward(float throttle)
{
	if (IsFpsMode == true)
	{
		AddMovementInput(VrCamera->GetForwardVector() * throttle);
	}
	else
	{
		if (LeftHand)
			AddMovementInput(LeftHand->GetForward() * throttle);
	}
}

void AEDPlayerCharacter::MoveRight(float throttle)
{
	if (IsFpsMode == true)
	{
		AddMovementInput(VrCamera->GetRightVector() * throttle);
	}
	else
	{
		if (LeftHand)
			AddMovementInput(LeftHand->GetRight() * throttle);
	}
}

void AEDPlayerCharacter::OnRep_HandsReplicated()
{
	if (LeftHand && RightHand && NetOwnerId != 0)
	{
		LeftHand->HandMeshComponent->SetWorldScale3D(FVector{ 1.0f, -1.0f, 1.0f });

		SetUpPlayerHeight();
		SetupBlinker();

		LeftHand->SetOwner(this);
		RightHand->SetOwner(this);
	}
}

void AEDPlayerCharacter::Server_UpdateMotionControllers_Implementation(const FMotionControllerReplicationData& leftHandData, const FMotionControllerReplicationData& rightHandData)
{
	LeftHand->SetActorLocation(leftHandData.GetPosition());
	LeftHand->SetActorRotation(leftHandData.GetRotation());
	RightHand->SetActorLocation(rightHandData.GetPosition());
	RightHand->SetActorRotation(rightHandData.GetRotation());
}

ED_DEFAULT_SERVER_VALIDATION(AEDPlayerCharacter, UpdateMotionControllers, const FMotionControllerReplicationData& leftHandTransform, const FMotionControllerReplicationData& rightHandTransform)

void AEDPlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEDPlayerCharacter, LeftHand);
	DOREPLIFETIME(AEDPlayerCharacter, RightHand);
	DOREPLIFETIME(AEDPlayerCharacter, NetOwnerId);
}
