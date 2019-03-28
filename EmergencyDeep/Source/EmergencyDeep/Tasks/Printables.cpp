
#include "Printables.h"
#include "Engine/World.h"
#include "MotionControllerComponent.h"
#include "Player/EDPlayerHand.h"
#include "Player/GrippableStaticMeshComponent.h"


// Sets default values
APrintables::APrintables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Package = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Package"));
	Package->SetSimulatePhysics(true);
	SetRootComponent(Package);

	//Package->SetCollisionProfileName(TEXT("Grippable"));
}

// Called when the game starts or when spawned
void APrintables::BeginPlay()
{
	Super::BeginPlay();
	PrevPos = Package->GetComponentLocation();
	if (IsTemplate)
	{
		//SetVisible(false);
	}
}


void APrintables::OnGrab_Implementation(USceneComponent * motionController, UPhysicsHandleComponent * PhysicsHandle, FVector GrabLocation)
{
	Package->AttachActor(motionController, GetRootComponent(), Package);
	IsGripped = true;
	PrevPos = Package->GetComponentLocation();
	MotionContr = motionController;
}

void APrintables::OnGrabRelease_Implementation(UPhysicsHandleComponent * PhysicsHandle)
{
	Package->DettachActor(GetRootComponent(),Package,true);
	IsGripped = false;
}

void APrintables::Setup(int vendingCode, TSubclassOf<AActor> spawn, float shakeLim, float sensitivity)
{
	Code = vendingCode;
	ToSpawn = spawn;
	ShakeLimit = shakeLim;
	ShakeSensitive = sensitivity;
}

void APrintables::GetToPrintObject(TSubclassOf<AActor>& object)
{
	object = ToSpawn;
}

bool APrintables::CheckCode(const TArray<int>& vendingCode)
{
	int digits = vendingCode.Num();
	if (digits == 0) return false;

	// Check right amount of digits
	int check = int(Code / pow(10, digits - 1));
	if (check >= 1 && check < 10)
	{
		int tempCode = Code;
		int index;

		// Check the code
		for (int i = 0; i < digits; i++)
		{
			int singleDigit = tempCode % 10;
			
			if (vendingCode.Find(singleDigit, index) == false)
			{
				return false;
			}

			tempCode /= 10;
		}
		
		// If we reach here that means the code is correct
		UE_LOG(LogTemp, Warning, TEXT("Code Succesfull %i"), Code);
		return true;
	}
	else return false;
}

void APrintables::SetVisible(bool visible)
{
	SetActorHiddenInGame(!visible);
	SetActorEnableCollision(visible);
}

void APrintables::Unpack()
{
	UE_LOG(LogTemp, Warning, TEXT("Unpack %i"),Code);
	// Spawn a button
	UWorld* world = GetWorld();
	FActorSpawnParameters spawnParams{};
	AActor* actor = world->SpawnActor<AActor>(ToSpawn, Package->GetComponentLocation(), Package->GetComponentRotation(), spawnParams);
	actor->SetActorScale3D(FVector::OneVector);
	
	// Try to grab it

	AEDPlayerHand* hand = Cast<AEDPlayerHand>(MotionContr->GetOwner());
	if (hand)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempt to grab"));
		hand->DropObject();
		hand->GrabObject();
	}
}

void APrintables::UpdateShake(float DeltaTime)
{
	if (IsTemplate == false)
	{
		if (IsGripped)
		{
			//update rotation

			FVector currentVelocity = (Package->GetComponentLocation() - PrevPos) / DeltaTime;
			float change = FVector::DotProduct(currentVelocity.GetSafeNormal(), PrevVelocity.GetSafeNormal());
			if (change <= ShakeSensitive && PrevVelocity.Size() >= 0)
			{
				Shake += DeltaTime * currentVelocity.Size();
			}
			else
			{
				Shake -= DeltaTime * 0.1f;
			}

			if (Shake <= 0)
			{
				Shake = 0;
			}
			else if (Shake >= ShakeLimit)
			{
				//Spawn actor
				Unpack();
				Destroy();
			}
			PrevPos = Package->GetComponentLocation();
			PrevVelocity = currentVelocity;
		}
		else
		{
			Shake = 0;
		}
	}
}

// Called every frame
void APrintables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateShake(DeltaTime);
}