#include "CircuitBoard.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Player/GrippableStaticMeshComponent.h"
#include "Engine/World.h"
#include "../Interactables/Wire.h"
#include "../Interactables/BaseLever.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


// Sets default values
ACircuitBoard::ACircuitBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FuseBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuseBox"));
	Door = CreateDefaultSubobject<UGrippableStaticMeshComponent>(TEXT("Door"));
	DoorClosedTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorClosedTrigger"));
	DoorConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DoorConstraint"));
	ConstraintPositioner = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPositioner"));
	RootWires = CreateDefaultSubobject<USceneComponent>(TEXT("RootWires"));
	Lever = CreateDefaultSubobject<UChildActorComponent>(TEXT("Lever"));
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));

	RootComponent = FuseBox;
	Door->SetupAttachment(FuseBox);
	DoorClosedTrigger->SetupAttachment(RootComponent);
	ConstraintPositioner->SetupAttachment(RootComponent);
	DoorConstraint->SetupAttachment(ConstraintPositioner);
	RootWires->SetupAttachment(FuseBox);
	Lever->SetupAttachment(FuseBox);
	LightMesh->SetupAttachment(FuseBox);
	Light->SetupAttachment(LightMesh);

	Lever->CreateChildActor();


	Door->SetCollisionProfileName(TEXT("Grippable"));
	DoorClosedTrigger->SetCollisionProfileName(TEXT("GrippableTrigger"));
	Door->SetSimulatePhysics(true);
	Door->SetEnableGravity(false);
	Door->SetLinearDamping(100.0f);
	Door->SetAngularDamping(100.0f);

	DoorClosedTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACircuitBoard::OnBeginOverlap);
	DoorClosedTrigger->OnComponentEndOverlap.AddDynamic(this, &ACircuitBoard::OnEndOverlap);

	DoorConstraint->ComponentName1 = FConstrainComponentPropName{ FName{ "FuseBox" } };
	DoorConstraint->ComponentName2 = FConstrainComponentPropName{ FName{ "Door" } };

	Light->SetLightColor(FLinearColor::Green);
}

// Called when the game starts or when spawned
void ACircuitBoard::BeginPlay()
{
	Super::BeginPlay();

	DoorConstraint->SetConstraintReferencePosition(EConstraintFrame::Frame1, ConstraintPositioner->RelativeLocation);
	DoorConstraint->SetConstraintReferencePosition(EConstraintFrame::Frame2, FVector{ 0.0f });

	//DoorConstraint->SetConstraintReferenceOrientation(EConstraintFrame::Frame1, FVector{ 0.0f, 1.0f, 0.0f }, FVector{ -1.0f, 0.0f, 0.0f });
	DoorConstraint->SetConstraintReferenceOrientation(EConstraintFrame::Frame2, FVector{ 0.0f, -1.0f, 0.0f }, FVector{ 1.0f, 0.0f, 0.0f });

	DoorConstraint->SetLinearXLimit(LCM_Locked, 0.0f);
	DoorConstraint->SetLinearYLimit(LCM_Locked, 0.0f);
	DoorConstraint->SetLinearZLimit(LCM_Locked, 0.0f);
	DoorConstraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	DoorConstraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	DoorConstraint->SetAngularSwing1Limit(ACM_Limited, DoorMaxAngle);
	
	// Wires
	SpawnWires();

	// Lever
	ABaseLever* leverT = Cast<ABaseLever>(Lever->GetChildActor());
	if (leverT)
	{
		leverT->OnTopTriggerEnterDelegate.AddDynamic(this, &ACircuitBoard::CheckConnections);
	}

	// Light
	LightBrightness = Light->Intensity;

	//AutoSolve();
}

// Called every frame
void ACircuitBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTime > 0)
	{
		CurrentTime -= DeltaTime;
	}

	if (!KickStarted && Fixed && CurrentTime>0)
	{
		KickStart();
	}
	UpdateLight(DeltaTime);
}

void ACircuitBoard::CheckConnections()
{
	if (Fixed == true)
	{
		CurrentTime = DelayKickstart;
	}
	bool broken = false;
	for (int i = 0; i < Wires.Num(); i++)
	{
		if (Wires[i]->ValidateConnection() == false)
		{
			broken = true;
		}
	}
	if (broken == false)
	{
		Fixed = true;
		CurrentTime = DelayKickstart;
		Light->SetIntensity(LightBrightness*0.6f);
		OnSolved_BP();
	}
	else
	{
		OnNotSolved_BP();
	}
}

void ACircuitBoard::KickStart()
{
	if (!KickStarted && Fixed)
	{
		if (LinkedCirquits.Num() == 0)
		{
			KickStarted = true;
			if (LightKickStart == nullptr)
			{
				Light->SetIntensity(LightBrightness);
			}
			else
			{
				CurrentLightTimeStart = 0;
			}
			OnKickStarted_BP();
			return;
		}
		else
		{
			bool check = true;
			for (int i = 0; i < LinkedCirquits.Num(); i++)
			{
				if (LinkedCirquits[i]->CurrentTime <= 0 && LinkedCirquits[i]->KickStarted == false)
				{
					check = false;
					Light->SetIntensity(0);
					OnKickStartFail_BP();
				}
			}
			if (check)
			{
				KickStarted = true;
				if (LightKickStart == nullptr)
				{
					Light->SetIntensity(LightBrightness);
				}
				else
				{
					CurrentLightTimeStart = 0;
				}
				OnKickStarted_BP();
			}
			
		}
	}
}

void ACircuitBoard::BreakFuseBox(bool kickStartOnly)
{
	KickStarted = false;
	if (kickStartOnly == false)
	{
		for (int i = 0; i < NrWireCouples; i++)
		{
			Wires[i]->DisConnect();
		}
		Fixed = false;
	}
	OnBreakFusebox_BP(!kickStartOnly);

	if (LightShutDown == nullptr)
	{
		Light->SetIntensity(0);
	}
	else
	{
		CurrentLightTimeShut = 0;
	}
}

void ACircuitBoard::AutoSolve()
{
	for (int i = 0; i < NrWireCouples; i++)
	{
		AWire* otherWire = nullptr;
		for (int j = NrWireCouples; j < (NrWireCouples * 2); j++)
		{
			if (Wires[j]->Id == i)
			{
				otherWire = Wires[j];
				j = NrWireCouples * 2;
			}
		}
		Wires[i]->Connect(otherWire);
	}
	Fixed = true;
	KickStarted = true;
	Light->SetIntensity(LightBrightness);
}

void ACircuitBoard::OnBeginOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherComp == Door)
	{
		OnClosePanel_BP();
	}
}

void ACircuitBoard::OnEndOverlap(UPrimitiveComponent * overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex)
{
	if (otherComp == Door)
	{
		OnOpenPanel_BP();
	}
}

void ACircuitBoard::SpawnWires()
{
	if (ToSpawnWire)
	{
		FVector startPos = FVector::ZeroVector;
		FVector newPos = startPos;
		UWorld* world = GetWorld();
		int right = 0; //Right Wire

		//Spawn
		for (int i = 0; i < NrWireCouples * 2; i++)
		{
			newPos = WirePlacements[i];
			
			FActorSpawnParameters spawnParams{};
			spawnParams.Owner = this;
			AWire* wire = world->SpawnActor<AWire>(ToSpawnWire, newPos, FRotator{NULL}, spawnParams);
			FAttachmentTransformRules attachmentRules{ EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,false };
			wire->AttachToComponent(RootWires, attachmentRules);
		
			if (wire != nullptr)
			{
				wire->Id = -1;
				Wires.Add(wire);
				if (i >= NrWireCouples)
				{
					wire->IsConnector = true;
				}
			}
			
		}

		//Connect
		for (int i = 0; i < NrWireCouples; i++)
		{
			//Step 1 check if there are any connections left
			int temp = int(CollorLegend.Num() / 2);
			TArray<int> possCons;
			int failedLink;
			
			//Get possible connections
			possCons = CheckLinks(i);
			
			//If any possible connection try connecting
			if (possCons.Num() > 0)
			{
				//Step 2 get random connection if available
				int t = rand() % possCons.Num();
				int id = possCons[t];
				// link wire
				Wires[i]->Id = i;
				Wires[i + id + NrWireCouples]->Id = i;
				//color wire
				Wires[i]->SetColor(CollorLegend[id + temp]);
				Wires[i + id + NrWireCouples]->SetColor(CollorLegend[id + temp]);

			}
			else
			{
				//If no connections possible
				//Dump one
				failedLink = i;
				bool linked = false;
				do
				{

					RelinkWire(failedLink, failedLink);

					possCons = CheckLinks(failedLink);

					if (possCons.Num() > 0)
					{
						//Step 2 get random connection if available
						int t = rand() % possCons.Num();
						int id = possCons[t];
						// link wire
						Wires[failedLink]->Id = failedLink;
						Wires[failedLink + id + NrWireCouples]->Id = failedLink;
						//color wire
						Wires[failedLink]->SetColor(CollorLegend[id + temp]);
						Wires[failedLink + id + NrWireCouples]->SetColor(CollorLegend[id + temp]);
						linked = true;
					}
					//Else Relink again, get more connections

				} while (linked == false);
			}

		}

	}
}

TArray<int> ACircuitBoard::CheckLinks(int wireI)
{
	TArray<int> possCons;
	int temp = int(CollorLegend.Num() / 2);
	for (int j = 0; j < CollorLegend.Num(); j++)
	{
		int id = j - temp;
		if (wireI + id >= 0 && wireI + id < NrWireCouples)
		{
			if (Wires[wireI + id + NrWireCouples]->Id == -1)
			{
				possCons.Add(id);
			}
		}
	}
	return possCons;
}

void ACircuitBoard::RelinkWire(int wireId, int& failedLink)
{
	int temp = int(CollorLegend.Num() / 2);
	bool find = false;
	do
	{
		int id = rand() % (CollorLegend.Num() - temp) - temp;
		if (wireId + id >= 0 && wireId + id < NrWireCouples)
		{
			failedLink = (Wires[wireId + id + NrWireCouples]->Id);
			// link wire
			Wires[wireId]->Id = wireId;
			Wires[wireId + id + NrWireCouples]->Id = wireId;
			//color wire
			Wires[wireId]->SetColor(CollorLegend[id + temp]);
			Wires[wireId + id + NrWireCouples]->SetColor(CollorLegend[id + temp]);

			find = true;
		}

	} while (find == false);
}

void ACircuitBoard::UpdateLight(float deltaTime)
{
	if (CurrentLightTimeShut < MaxLightShutDownTime && LightShutDown != nullptr)
	{
		CurrentLightTimeShut += deltaTime;
		float brightness = LightShutDown->GetFloatValue(CurrentLightTimeShut);
		Light->SetIntensity(brightness);
	}
	if (CurrentLightTimeStart < MaxLightKickStartTime && LightKickStart != nullptr)
	{
		CurrentLightTimeStart += deltaTime;
		float brightness = LightKickStart->GetFloatValue(CurrentLightTimeStart);
		Light->SetIntensity(brightness);
	}
}

void ACircuitBoard::OnGrab_Implementation(USceneComponent * MotionController, UPhysicsHandleComponent * PhysicsHandle, FVector GrabLocation)
{
	Door->AttachComponent(MotionController, PhysicsHandle, GrabLocation);
}

void ACircuitBoard::OnGrabRelease_Implementation(UPhysicsHandleComponent * PhysicsHandle)
{
	Door->DettachComponent(PhysicsHandle);
}

