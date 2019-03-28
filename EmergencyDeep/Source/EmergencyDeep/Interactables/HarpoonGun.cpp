#include "HarpoonGun.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/World.h"
#include "Props/BaseProjectile.h"
#include "UnrealNetwork.h"

AHarpoonGun::AHarpoonGun() : ShootTimer(0.0f), CanShoot(true), bIsGrabbedByPlayer(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	bReplicateMovement = true;

	HarpoonGunCabin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarpoonGunCabin"));
	HarpoonGunBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarpoonGunBarrel"));
	HarpoonGunBarrelEnd = CreateDefaultSubobject<USceneComponent>(TEXT("HarpoonGunBarrelEnd"));
	HarpoonGunHandle = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("HarpoonGunHandle"));
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));
	AmmoLoadingArea = CreateDefaultSubobject<UBoxComponent>(TEXT("AmmoLoadingArea"));

	RootComponent = HarpoonGunCabin;
	HarpoonGunHandle->SetupAttachment(HarpoonGunCabin);
	ConstraintPositioner->SetupAttachment(RootComponent);
	Constraint->SetupAttachment(ConstraintPositioner);
	AmmoLoadingArea->SetupAttachment(HarpoonGunCabin);

	HarpoonGunCabin->SetCollisionProfileName(TEXT("OverlappAll"));
	HarpoonGunHandle->SetCollisionProfileName(TEXT("Interactable"));
	HarpoonGunHandle->SetSimulatePhysics(true);
	HarpoonGunHandle->SetEnableGravity(false);
	HarpoonGunHandle->SetLinearDamping(1000.0f);
	HarpoonGunHandle->SetAngularDamping(1000.0f);

	HarpoonGunBarrel->SetupAttachment(HarpoonGunHandle);
	HarpoonGunBarrelEnd->SetupAttachment(HarpoonGunBarrel);

	Constraint->ComponentName1 = FConstrainComponentPropName{ FName{ "HarpoonGunCabin" } };
	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{ "HarpoonGunHandle" } };

	AmmoLoadingArea->OnComponentBeginOverlap.AddDynamic(this, &AHarpoonGun::OnBeginOverlap);

	LoadedAmmo = 10;
	MaxLoadedAmmo = 10;
	FireRate = 1.f; // 1 harpoon per second by default
}

void AHarpoonGun::BeginPlay()
{
	Super::BeginPlay();

	Constraint->SetLinearXLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearYLimit(LCM_Locked, 0.0f);
	Constraint->SetLinearZLimit(LCM_Locked, 0.0f);

	Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
}

void AHarpoonGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanShoot)
	{
		ShootTimer += DeltaTime;
		if (ShootTimer >= 1.0f / FireRate)
		{
			CanShoot = true;
			OnInternalReloadEnd_BP();
		}
	}
}

void AHarpoonGun::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	HarpoonGunHandle->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
	bIsGrabbedByPlayer = true;
}

void AHarpoonGun::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	HarpoonGunHandle->DettachComponent(PhysicsHandle);
	bIsGrabbedByPlayer = false;
}

void AHarpoonGun::Shoot_Implementation()
{
	if (HarpoonActor)
	{
		UWorld *world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			FVector spawnLocation = HarpoonGunBarrelEnd->GetComponentLocation();
			FVector forward = HarpoonGunBarrelEnd->GetForwardVector();
			FRotator rotation = forward.Rotation();

			world->SpawnActor<ABaseProjectile>(HarpoonActor, spawnLocation, rotation, spawnParams);

			OnShoot_BP();

			if(HasAuthority())
				--LoadedAmmo;

			if(LoadedAmmo <= 0)
				OnOutOfAmmo_BP();
		}
	}
}

void AHarpoonGun::TryShoot_Implementation()
{
	if (CanShoot)
	{
		Shoot();
		CanShoot = false;
		ShootTimer = 0.0f;
		OnInternalReloadStart_BP();
		return;
	}
	OnOutOfAmmo_BP();
}

void AHarpoonGun::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor != this)
	{
		auto projectile = Cast<ABaseProjectile>(otherActor);
		if (projectile)
		{
			if (projectile->bReadyForDelete)
			{
				if (LoadedAmmo < MaxLoadedAmmo)
				{
					GetWorld()->DestroyActor(projectile);

					if (HasAuthority())
						++LoadedAmmo;

					OnAmmoInsertion_BP();
				}
			}
		}
	}
}

void AHarpoonGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHarpoonGun, LoadedAmmo)
}
