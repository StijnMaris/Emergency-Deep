#include "BrokenPipe.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../../Player/GrippableStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OilSpill.h"
#include "NewPipe.h"
#include "Hammer.h"
#include "Interactables/BaseWheel.h"
#include "Tasks/EDTaskManager.h"
#include "Player/EDPlayerHand.h"

ABrokenPipe::ABrokenPipe() :Scale(1.f), ValveScale(1.f), PipeConnectorScale(1.f), IsPipeFixed(true), RequiredHitCount(2), BreakDelay(5.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ValvePipe = CreateDefaultSubobject<UChildActorComponent>(TEXT("ValvePipe"));
	PipePosition = CreateDefaultSubobject<USceneComponent>(TEXT("PipePosition"));
	PipePart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipePart"));
	PipePartBroken = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipePartBroken"));
	Oil = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Oil"));
	TriggerBoxFixFar = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxFixFar"));
	TriggerBoxFixClose = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxFixClose"));
	TriggerBoxLiquid = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxLiquid"));

	RootComponent = Root;
	ValvePipe->SetupAttachment(Root);
	PipePosition->SetupAttachment(ValvePipe);
	PipePart->SetupAttachment(PipePosition);
	PipePartBroken->SetupAttachment(PipePosition);

	Oil->SetupAttachment(PipePosition);
	TriggerBoxLiquid->SetupAttachment(PipePosition);
	TriggerBoxFixClose->SetupAttachment(PipePosition);
	TriggerBoxFixFar->SetupAttachment(PipePosition);

	PipePart->SetIsReplicated(true);
	PipePartBroken->SetIsReplicated(true);
	//Oil->SetIsReplicated(true);

	PipePartBroken->SetVisibility(false);

	PipePartBroken->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//ValvePipe->SetChildActorClass(ABaseWheel::StaticClass());

	TriggerBoxLiquid->OnComponentBeginOverlap.AddDynamic(this, &ABrokenPipe::OnBeginOverlapLiquid);
	TriggerBoxLiquid->OnComponentEndOverlap.AddDynamic(this, &ABrokenPipe::OnEndOverlapLiquid);

	TriggerBoxFixFar->OnComponentBeginOverlap.AddDynamic(this, &ABrokenPipe::OnBeginOverlapFar);
	TriggerBoxFixFar->OnComponentEndOverlap.AddDynamic(this, &ABrokenPipe::OnEndOverlapFar);

	TriggerBoxFixClose->OnComponentBeginOverlap.AddDynamic(this, &ABrokenPipe::OnBeginOverlapClose);
	TriggerBoxFixClose->OnComponentEndOverlap.AddDynamic(this, &ABrokenPipe::OnEndOverlapClose);
}

void ABrokenPipe::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Cast<ABaseWheel>(ValvePipe->GetChildActor())->bLimitValveRotation = true;
	Cast<ABaseWheel>(ValvePipe->GetChildActor())->ValveMaxAngle = 150;
	Cast<ABaseWheel>(ValvePipe->GetChildActor())->RotationAxis = ERotationAxis::RA_Y;
}

void ABrokenPipe::BeginPlay()
{
	Super::BeginPlay();
	if (Cast<ABaseWheel>(ValvePipe->GetChildActor()))
	{
		Cast<ABaseWheel>(ValvePipe->GetChildActor())->SetActorScale3D(FVector{ PipeConnectorScale });
		Cast<ABaseWheel>(ValvePipe->GetChildActor())->ValveMesh->SetWorldScale3D(FVector{ ValveScale });
		PipePart->SetWorldScale3D(FVector{ Scale });
		PipePartBroken->SetWorldScale3D(FVector{ Scale,Scale,0.2 });
	}

	//Root->SetWorldScale3D(FVector{ Scale });

	//FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABrokenPipe::BrakePipe, BreakDelay, false);
}

void ABrokenPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsPipeFixed && OilSpill && Cast<ABaseWheel>(ValvePipe->GetChildActor()) && HasAuthority())
	{
		if (Cast<ABaseWheel>(ValvePipe->GetChildActor())->IsValveOn())
		{
			OilSpill->OilGrowState = EGrowState::Idle;
			OilSpill->IsValveOpen = false;
			IsOilOn = false;
		}
		else
		{
			OilSpill->OilGrowState = EGrowState::Growing;
			OilSpill->IsValveOpen = true;
			IsOilOn = true;
		}
	}

	if (!IsOilOn)
	{
		Oil->Deactivate();
	}
	else
	{
		Oil->Activate();
	}

	if (HasAuthority()) {
		if (!OilSpill)
		{
			IsOilCleaned = true;
		}
	}
}

void ABrokenPipe::BrakePipe()
{
	OnPipeBreak();

	IsPipeFixed = false;
	IsOilCleaned = false;

	PipePart->SetVisibility(false);
	//Cast<ABaseWheel>(ValvePipe->GetChildActor())->WheelBase->SetVisibility(false);

	PipePart->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Cast<ABaseWheel>(ValvePipe->GetChildActor())->WheelBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PipePartBroken->SetVisibility(true);

	PipePartBroken->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Oil->Activate();

	FVector start = Oil->GetComponentLocation();
	FVector right = Oil->GetRightVector();

	start += right * 100;

	FVector end = start;
	end.Z -= 200;

	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.bTraceAsyncScene = true;
	traceParams.bReturnPhysicalMaterial = false;

	FHitResult hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		hit,
		start,
		end,
		ECC_Visibility,
		traceParams
	);

	FVector Location(hit.ImpactPoint);
	FRotator Rotation(0.0f, 90.0f, 0.0f);
	FActorSpawnParameters SpawnInfo{};
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	OilSpill = GetWorld()->SpawnActor<AOilSpill>(OilSpillBP, Location, Rotation, SpawnInfo);
}

//bool ABrokenPipe::Server_BrakePipe_Validate()
//{
//	return true;
//}
//
//void ABrokenPipe::Server_BrakePipe_Implementation()
//{
//
//}

void ABrokenPipe::FixPipe()
{
	IsPipeFixed = true;
	PipePartBroken->SetVisibility(false);

	PipePartBroken->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PipePart->SetVisibility(true);
	//Cast<ABaseWheel>(ValvePipe->GetChildActor())->WheelBase->SetVisibility(true);

	PipePart->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Cast<ABaseWheel>(ValvePipe->GetChildActor())->WheelBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	OnPipeFix();
	NewPipeLeft->Destroy();
	//NewPipeLeft = nullptr;
	NewPipeRight = nullptr;
	NewPipe = nullptr;
}

void ABrokenPipe::CheckRequirements()
{
	if (Cast<ABaseWheel>(ValvePipe->GetChildActor())->IsValveOn() &&
		HitCountRight > RequiredHitCount &&
		HitCountLeft > RequiredHitCount &&
		NewPipeLeft &&
		NewPipeRight &&
		Hammer && !IsPipeFixed)
	{
		FixPipe();
	}
}

bool ABrokenPipe::IsComplete()
{
	if (IsPipeFixed && IsOilCleaned)
	{
		return true;
	}
	return false;
}

//void ABrokenPipe::OnPipeBreak()
//{
//}
//
//void ABrokenPipe::OnPipeFix()
//{
//}
//
//void ABrokenPipe::OnHammerHit()
//{
//}

void ABrokenPipe::OnBeginOverlapLiquid(UPrimitiveComponent *overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (Cast<ANewPipe>(otherActor))
	{
		NewPipe = Cast<ANewPipe>(otherActor);
		ABaseWheel* valve = Cast<ABaseWheel>(ValvePipe->GetChildActor());
		if (NewPipe && !IsPipeFixed && !valve->IsValveOn())
		{
			if (NewPipe->HandRight)
			{
				AEDPlayerHand* hand = Cast<AEDPlayerHand>(NewPipe->HandRight->GetOwner());
				if (hand)
				{
					hand->DropObject();
					UGrippableStaticMeshComponent* mesh = Cast<UGrippableStaticMeshComponent>(NewPipe->GetComponentByClass(UGrippableStaticMeshComponent::StaticClass()));
					mesh->AddImpulse(Oil->GetUpVector() * 1000);
				}
			}
		}
	}
}

void ABrokenPipe::OnEndOverlapLiquid(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	NewPipe = nullptr;
}

void ABrokenPipe::OnBeginOverlapFar(UPrimitiveComponent *overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (Cast<ANewPipe>(otherActor))
	{
		NewPipeLeft = Cast<ANewPipe>(otherActor);
	}
	if (Cast<AHammer>(otherActor))
	{
		Hammer = Cast<AHammer>(otherActor);
		if (NewPipeLeft && NewPipeRight)
		{
			HitCountLeft++;
			OnHammerHit();
		}
	}
	CheckRequirements();
}

void ABrokenPipe::OnEndOverlapFar(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (Cast<ANewPipe>(otherActor))
	{
		NewPipeLeft = nullptr;
	}
	if (Cast<AHammer>(otherActor))
	{
		Hammer = nullptr;
	}
}

void ABrokenPipe::OnBeginOverlapClose(UPrimitiveComponent *overlappedComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (Cast<ANewPipe>(otherActor))
	{
		NewPipeRight = Cast<ANewPipe>(otherActor);
	}
	if (Cast<AHammer>(otherActor))
	{
		Hammer = Cast<AHammer>(otherActor);
		if (NewPipeLeft && NewPipeRight)
		{
			HitCountRight++;
			OnHammerHit();
		}
	}
	CheckRequirements();
}

void ABrokenPipe::OnEndOverlapClose(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (Cast<ANewPipe>(otherActor))
	{
		NewPipeRight = nullptr;
	}
	if (Cast<AHammer>(otherActor))
	{
		Hammer = nullptr;
	}
}

void ABrokenPipe::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABrokenPipe, TriggerBoxFixFar);
	DOREPLIFETIME(ABrokenPipe, PipePart);
	DOREPLIFETIME(ABrokenPipe, PipePartBroken);
	DOREPLIFETIME(ABrokenPipe, ValvePipe);
	DOREPLIFETIME(ABrokenPipe, TriggerBoxFixClose);
	DOREPLIFETIME(ABrokenPipe, TriggerBoxLiquid);
	DOREPLIFETIME(ABrokenPipe, OilSpill);
	DOREPLIFETIME(ABrokenPipe, NewPipe);
	DOREPLIFETIME(ABrokenPipe, NewPipeLeft);
	DOREPLIFETIME(ABrokenPipe, NewPipeRight);
	DOREPLIFETIME(ABrokenPipe, Hammer);
	DOREPLIFETIME(ABrokenPipe, HitCountLeft);
	DOREPLIFETIME(ABrokenPipe, HitCountRight);
	DOREPLIFETIME(ABrokenPipe, IsPipeFixed);
	DOREPLIFETIME(ABrokenPipe, IsOilCleaned);
	DOREPLIFETIME(ABrokenPipe, IsPipeInRightPosition);
	DOREPLIFETIME(ABrokenPipe, IsValveActivated);
	DOREPLIFETIME(ABrokenPipe, IsOilOn);
}