
#include "Wire.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CableComponent.h"
#include "MotionControllerComponent.h"
#include "Player/EDPlayerHand.h"
#include "Math/Color.h"


// Sets default values
AWire::AWire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	CableGrab = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("CableGrab"));
	WireTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("WireTrigger"));
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));

	RootComponent = Root;
	CableGrab->SetupAttachment(Root);
	WireTrigger->SetupAttachment(Root);
	Constraint->SetupAttachment(CableGrab);
	Cable->SetupAttachment(Root);

	CableGrab->SetSimulatePhysics(true);
	CableGrab->SetEnableGravity(false);

	WireTrigger->SetCollisionProfileName(TEXT("GrippableTrigger"));
	CableGrab->SetCollisionProfileName(TEXT("Grippable"));

	WireTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWire::OnBeginOverlap);
	WireTrigger->OnComponentEndOverlap.AddDynamic(this, &AWire::OnEndOverlap);

	Constraint->ComponentName2 = FConstrainComponentPropName{ FName{"CableGrab"} };
}

// Called when the game starts or when spawned
void AWire::BeginPlay()
{
	Super::BeginPlay();

	// Constraint

	Constraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	Constraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	Constraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);

	Constraint->SetLinearXLimit(LCM_Free, 0.0f);
	Constraint->SetLinearYLimit(LCM_Free, 0.0f);
	Constraint->SetLinearZLimit(LCM_Free, 0.0f);

	// Cable

	Cable->SetAttachEndTo(this, FName("CableGrab"));
	Cable->CableWidth = WireWidth;
	Cable->CableLength = WireLength;
	Cable->SetHiddenInGame(true);

	if (WireMaterialBase != nullptr)
	{
		WireMaterialInstance = UMaterialInstanceDynamic::Create(WireMaterialBase, this);
		Cable->SetMaterial(0, WireMaterialInstance);
	
	}
}

// Called every frame
void AWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckDistance();
}

void AWire::OnGrab_Implementation(USceneComponent * MotionController, UPhysicsHandleComponent * PhysicsHandle, FVector GrabLocation)
{
	if (IsConnected == false && IsConnector == false)
	{
		CableGrab->AttachComponent(MotionController, PhysicsHandle, CableGrab->GetComponentLocation());
		MotionContr = MotionController;
	}
}

void AWire::OnGrabRelease_Implementation(UPhysicsHandleComponent * PhysicsHandle)
{
	if (IsConnected == false && IsConnector == false)
	{
		CableGrab->DettachComponent(PhysicsHandle);
		MotionContr = nullptr;
		if (LastOverlappingWire != nullptr)
		{
			Connect(LastOverlappingWire);
		}
		else
		{
			CableGrab->SetWorldLocation(Root->GetComponentLocation());
			Cable->SetHiddenInGame(true);
		}
	}
		LastOverlappingWire = nullptr;

}
void AWire::SetColor(FLinearColor col)
{
	Color = col;
	WireMaterialInstance->SetVectorParameterValue(FName("Color"), Color);
}
void AWire::Connect(AWire * otherWire)
{
	// Connects this wire to the other
	// Other wire does not need to connect but just has to know

	// Connect this cable
	CableGrab->SetWorldLocation(otherWire->GetActorLocation());
	IsConnected = true;
	ConnectedWire = otherWire;

	// Connect other cable
	otherWire->IsConnected = true;
	otherWire->ConnectedWire = this;
}

void AWire::DisConnect()
{
	// Disconnect both wires
	if (IsConnected)
	{
		IsConnected = false;
		CableGrab->SetWorldLocation(Root->GetComponentLocation());
		Cable->SetHiddenInGame(true);

		// Disconnect other wire
		ConnectedWire->DisConnect();

		ConnectedWire = nullptr;

	}
}

AWire * AWire::GetConnectedWire()
{
	if (ConnectedWire != nullptr)
	{
		return ConnectedWire;
	}
	return nullptr;
}

AWire* AWire::GetLastOverlappingWire()
{
	if (LastOverlappingWire != nullptr)
	{
		return LastOverlappingWire;
	}
	return nullptr;
}

void AWire::SetLastOverlappingWire(AWire * otherWire)
{
	LastOverlappingWire = otherWire;
}

// Returns TRUE if the cable is connected to the right cable.
// Returns FALSE if the cable is not connected to any cable.
// Returns FALSE if the cable is connected to the wrong cable and Disconnects itself.
bool AWire::ValidateConnection()
{
	//Validates connection to see if connection is correct if not connection breaks
	if (IsConnected == false)
	{
		return false;
	}
	else if (Id == ConnectedWire->Id)
	{
		return true;
	}
	else
	{
		DisConnect();
		return false;
	}
}

bool AWire::CheckDistance()
{
	float distance = FVector::Distance(CableGrab->GetComponentLocation(), GetActorLocation());
	if (distance > MaxWireLength && MotionContr != nullptr)
	{
		//Break line
		AEDPlayerHand* hand = Cast<AEDPlayerHand>(MotionContr->GetOwner());
		if (hand)
		{
			hand->DropObject();

			// OnGrabrelease should trigger
		}
		
		return false;
	}
	else if (distance > 2 && MotionContr == nullptr && IsConnected == false)
	{
		CableGrab->SetWorldLocation(Root->GetComponentLocation());
		Cable->SetHiddenInGame(true);
		return false;
	}
	else return true;

}

void AWire::OnBeginOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherComp->GetOwner() == this)
	{
		if (otherComp == CableGrab)
		{
			// WireEnd to close to origin -> wire hidden
			Cable->SetHiddenInGame(true);
		}
	}
	else 
	{
		// Check if other actor is also a Wire to connect to
		AWire* otherWire = Cast<AWire>(otherActor);
		if (otherWire != nullptr)
		{
			//Check if same owner/fusebox
			if (this->GetOwner() != otherActor->GetOwner()) return;

			// Check if this wire is not connected
			if (otherWire->IsConnected || IsConnected)	return;

			// Check if we are overlapping with the gripcomponent and not the cable
			if (otherComp == otherWire->CableGrab)
			{
				otherWire->SetLastOverlappingWire(this);
				//Its better to set the property of the other wire
				//as the player is using that wire to confirm connection
			}
		}
	}
}

void AWire::OnEndOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex)
{
	if (otherComp == CableGrab)
	{
		// WireEnd has enough distance from origin -> make wire visible
		Cable->SetHiddenInGame(false);
	}
	else
	{
		if (LastOverlappingWire!=nullptr && LastOverlappingWire == otherActor)
		{
			AWire* otherWire = Cast<AWire>(otherActor);
			if (otherWire != nullptr)
			{
				otherWire->SetLastOverlappingWire(nullptr);
			}
		}
	}
}


