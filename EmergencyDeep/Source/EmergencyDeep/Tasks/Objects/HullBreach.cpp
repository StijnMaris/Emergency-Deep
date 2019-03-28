#include "HullBreach.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interactables/Cork.h"


// Sets default values
AHullBreach::AHullBreach()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HullBreachMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HullBreachMesh"));
	AttackTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackTrigger"));
	CorkTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CorkTrigger"));

	LeakParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEmitter"));



	RootComponent = HullBreachMesh;
	AttackTrigger->SetupAttachment(HullBreachMesh);
	CorkTrigger->SetupAttachment(HullBreachMesh);
	LeakParticleEmitter->SetupAttachment(HullBreachMesh);


	
	HullBreachMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	AttackTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHullBreach::OnBeginOverlapAttack);
	CorkTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHullBreach::OnBeginOverlapCork);

}

// Called when the game starts or when spawned
void AHullBreach::BeginPlay()
{
	Super::BeginPlay();

	LeakParticleEmitter->Deactivate();
	HullBreachMesh->SetHiddenInGame(true);

}

void AHullBreach::OnBeginOverlapAttack(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherActor->ActorHasTag("Enemy"))
	{
		Damage();
	}

}

void AHullBreach::OnBeginOverlapCork(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{

	ACork* actor = Cast<ACork>(otherActor);
	if (actor != nullptr)
	{
		CloseBreach(actor);
	}
}

void AHullBreach::CloseBreach(ACork* actor)
{
	if (Broken == true && Corked == false)
	{
		Cork = actor;
		actor->CloseLeak(this);
		Corked = true;
		LeakParticleEmitter->Deactivate();
		OnCorked_BP();
	}
}

// Called every frame
void AHullBreach::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHullBreach::Damage()
{
	if (Broken == false)
	{
		HullBreachMesh->SetHiddenInGame(false);
		Broken = true;
		OnBreached_BP();

	}
	if (Cork != nullptr)
	{
		Cork->CreateLeak();
		Cork = nullptr;
		OnUnCorked_BP();
	}
	Corked = false;
	LeakParticleEmitter->Activate();
}

bool AHullBreach::IsLeaking()
{
	if (Broken == false) return false;
	if (Corked == false) return true;
	return false;

}

