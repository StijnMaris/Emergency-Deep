#include "OilSpill.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "UnrealNetwork.h"

AOilSpill::AOilSpill()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	OilSpill = CreateDefaultSubobject<UDecalComponent>(TEXT("OilSpill"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Flames = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flames"));

	SceneRoot->SetupAttachment(RootComponent);
	OilSpill->SetupAttachment(SceneRoot);
	CollisionBox->SetupAttachment(OilSpill);
	Flames->SetupAttachment(SceneRoot);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AOilSpill::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AOilSpill::OnEndOverlap);

	OilSpill->SetIsReplicated(true);

	GrowSpeed = 0;
	CleaningSpeed = -20;
	OilScale = 0;
	FireScale = 0;
	OriginalGrowSpeed = 5;
	GrowSpeed = 0;
	CatchFireDelay = 60;
	Mop = nullptr;
	IsOnFire = false;
	IsValveOpen = true;
	OilGrowState = EGrowState::Idle;
	FireGrowState = EGrowState::Idle;
	OxigenDepletionRate = 0.1;
}

void AOilSpill::BeginPlay()
{
	Super::BeginPlay();
	OilGrowState = EGrowState::Growing;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOilSpill::CatchFire, CatchFireDelay, false);
}

void AOilSpill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOnFire)
	{
		Flames->Activate();
	}
	else
	{
		Flames->Deactivate();
	}

	if (Mop && !IsOnFire)
	{
		CheckIfMopIsCleening();
	}

	if ((OilGrowState == EGrowState::Growing || OilGrowState == EGrowState::Shrinking) && HasAuthority())
	{
		if (OilGrowState == EGrowState::Growing)
		{
			GrowSpeed = OriginalGrowSpeed;
		}
		else if (OilGrowState == EGrowState::Shrinking)
		{
			GrowSpeed = CleaningSpeed;
		}

		OilScale = UpdateScale(DeltaTime, OilScale);

		if (OilGrowState == EGrowState::Shrinking && OilScale < 0.1)
		{
			Destroy();
		}

		GrowAndShrinkOil();
	}

	if (FireGrowState == EGrowState::Growing || FireGrowState == EGrowState::Shrinking)
	{
		if (FireGrowState == EGrowState::Growing)
		{
			GrowSpeed = OriginalGrowSpeed;
		}
		else if (FireGrowState == EGrowState::Shrinking)
		{
			GrowSpeed = CleaningSpeed;
		}

		FireScale = UpdateScale(DeltaTime, FireScale);
		if (FireScale > OilScale)
		{
			FireScale = OilScale;
		}

		if (FireGrowState == EGrowState::Shrinking && FireScale < 0.1)
		{
			StopFire();
		}
		GrowAndShrinkFire();
	}
}

void AOilSpill::CatchFire()
{
	IsOnFire = true;
	FireGrowState = EGrowState::Growing;
	if (Mop)
	{
		Mop->CatchFire();
	}

	if (IsValveOpen)
	{
		OilGrowState = EGrowState::Growing;
	}
	else
	{
		OilGrowState = EGrowState::Idle;
	}
	OnCatchFire();
}

void AOilSpill::StopFire()
{
	IsOnFire = false;
	Flames->Deactivate();
	FireGrowState = EGrowState::Idle;
	OnStopFire();
}

void AOilSpill::GrowAndShrinkFire()
{
	Flames->SetVectorParameter(FName{ "Size" }, FVector{ FireScale,FireScale,FireScale });
}

float AOilSpill::UpdateScale(float DeltaTime, float scale)
{
	scale = ((DeltaTime / 100)* GrowSpeed) + scale;
	if (scale > 1)
	{
		scale = 1;
	}
	else if (scale < 0)
	{
		scale = 0;
	}
	return scale;
}

void AOilSpill::GrowAndShrinkOil()
{
	OilSpill->SetWorldScale3D(FVector{ 0.1,OilScale,OilScale });
}

void AOilSpill::CheckIfMopIsCleening()
{
	if (!Mop->IsOnFire)
	{
		if (Mop->GetCleaningPos().Equals(LastPos, 5.f) || IsValveOpen)
		{
			OilGrowState = EGrowState::Idle;
		}
		else
		{
			OilGrowState = EGrowState::Shrinking;
			OnCleaningOil();
		}
		LastPos = Mop->GetActorLocation();
	}
	else
	{
		OilGrowState = EGrowState::Growing;
	}
}

void AOilSpill::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	
	if (Cast<AMop>(otherActor))
	{
		Mop = Cast<AMop>(otherActor);
		if (!IsOnFire && !Mop->IsOnFire)
		{
			OilGrowState = EGrowState::Idle;
		}
		else
		{
			Mop->CatchFire();
			OilGrowState = EGrowState::Growing;
		}
	}
}

void AOilSpill::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (Cast<AMop>(otherActor))
	{
		Mop = nullptr;
		OilGrowState = EGrowState::Growing;
	}
}

void AOilSpill::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOilSpill, OilSpill);
	DOREPLIFETIME(AOilSpill, GrowSpeed);
	DOREPLIFETIME(AOilSpill, OilScale);
	DOREPLIFETIME(AOilSpill, FireScale);
	DOREPLIFETIME(AOilSpill, CatchFireDelay);
	DOREPLIFETIME(AOilSpill, OxigenDepletionRate);
	DOREPLIFETIME(AOilSpill, Mop);
	DOREPLIFETIME(AOilSpill, LastPos);
	DOREPLIFETIME(AOilSpill, IsOnFire);
	DOREPLIFETIME(AOilSpill, IsValveOpen);
	DOREPLIFETIME(AOilSpill, OilGrowState);
	DOREPLIFETIME(AOilSpill, FireGrowState);
}