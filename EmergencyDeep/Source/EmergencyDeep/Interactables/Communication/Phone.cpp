#include "Phone.h"
#include "Components/StaticMeshComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "CableComponent.h"
#include "Player/EDPlayerHand.h"
#include "PhoneHorn.h"

APhone::APhone()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	PhoneBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhoneBaseMesh"));
	PhoneCable = CreateDefaultSubobject<UCableComponent>(TEXT("PhoneCable"));
	PhoneBaseCablePosition = CreateDefaultSubobject<USceneComponent>(TEXT("PhoneBaseCablePosition"));
	Horn = CreateDefaultSubobject<UChildActorComponent>(TEXT("Horn"));

	RootComponent = PhoneBaseMesh;
	PhoneCable->SetupAttachment(PhoneBaseCablePosition);
	PhoneBaseCablePosition->SetupAttachment(PhoneBaseMesh);
	Horn->SetupAttachment(PhoneBaseMesh);

	PhoneCable->CableWidth = 1.0f;
	PhoneCable->EndLocation = FVector{ 0.0f };
}

void APhone::BeginPlay()
{
	Super::BeginPlay();

	//PhoneCable->SetAttachEndTo(Horn, TEXT("PhoneHornCablePosition"));
}

void APhone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDetachCable();
}

void APhone::CheckDetachCable()
{
	APhoneHorn* horn = Cast<APhoneHorn>(Horn->GetChildActor());
	if (horn)
	{
		if (PhysicsHandleComponent && (PhoneBaseCablePosition->GetComponentLocation() - horn->PhoneHornCablePosition->GetComponentLocation()).Size() >= PhoneCable->CableLength)
		{
			horn->PhoneHornMesh->DettachComponent(PhysicsHandleComponent);
			PhysicsHandleComponent = nullptr;
		}
	}
}