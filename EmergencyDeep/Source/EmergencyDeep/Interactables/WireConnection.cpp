#include "WireConnection.h"
#include "Wire.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UnrealNetwork.h"

// Sets default values
AWireConnection::AWireConnection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = false;
	bReplicateMovement = true;

	ConnectionPin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectionPin"));
	RootComponent = ConnectionPin;

	Id = 0;
	RowId = 0;
}

// Called when the game starts or when spawned
void AWireConnection::BeginPlay()
{
	Super::BeginPlay();
}

void AWireConnection::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWireConnection, IsConnected)
	DOREPLIFETIME(AWireConnection, Id)
}

// Called every frame
void AWireConnection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

