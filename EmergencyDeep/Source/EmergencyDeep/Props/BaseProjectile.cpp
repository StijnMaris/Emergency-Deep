#include "BaseProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Interactables/HarpoonDispenser.h"
#include "AI/AIDamageComponent.h"

// Sets default values
ABaseProjectile::ABaseProjectile() : Velocity(0.0f, 0.0f, 0.0f), bInFlight(false), bReadyForDelete(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("ProjectileMesh"));
	HeadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadTrigger"));

	RootComponent = ProjectileMesh;
	HeadTrigger->SetupAttachment(ProjectileMesh);

	HeadTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnBeginOverlap);

	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(true);

	ProjectileSpeed = 0.0f;
	ProjectileDamage = 50.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileSpeed > 0.0f)
	{
		Velocity = GetActorForwardVector() * ProjectileSpeed;
		bInFlight = true;
	}

	if (!bInFlight) bReadyForDelete = true;
}

void ABaseProjectile::BeginDestroy()
{
	Super::BeginDestroy();

	if (Dispenser)
		Dispenser->ReduceSpawnCount();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInFlight)
	{
		if (Velocity.Size() > 1.0f)
		{
			auto location = GetActorLocation();
			location += Velocity;
			SetActorLocation(location);
		}
		else bInFlight = false;
	}
}

void ABaseProjectile::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	ProjectileMesh->AttachActor(MotionController, RootComponent, ProjectileMesh);
}

void ABaseProjectile::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	ProjectileMesh->DettachActor(RootComponent, ProjectileMesh, true);
}

void ABaseProjectile::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (!bInFlight) return;

	if (otherActor != this)
	{
		bInFlight = false;
		FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);

		AttachToActor(overlappedComp->GetAttachParent()->GetOwner(), attachRules);
		ProjectileMesh->SetSimulatePhysics(false);

		TArray<UAIDamageComponent*> DamageComps;
		otherActor->GetComponents(DamageComps);
		if (DamageComps.Num() > 0)
		{
			DamageComps[0]->DealDamage(ProjectileDamage);
		}
	}
}
