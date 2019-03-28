#include "DoubleGripStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EDPlayerHand.h"

void UDoubleGripStaticMeshComponent::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	if (PrimaryHand != nullptr)
	{
		SecondaryHand = Cast<AEDPlayerHand>(MotionController->GetOwner());
		
	}
	else
	{
		PrimaryHand = Cast<AEDPlayerHand>(MotionController->GetOwner());
		PrimaryGrabbedLocation = GrabLocation;
		RelativeOffset = PrimaryGrabbedLocation - GetComponentLocation();
		StartOrientation = GetForwardVector();
	}
}

void UDoubleGripStaticMeshComponent::OnGrabReleaseController_Implementation(UPhysicsHandleComponent* PhysicsHandle, USceneComponent* MotionController)
{
	AEDPlayerHand* hand = Cast<AEDPlayerHand>( MotionController->GetOwner());
	if (hand == SecondaryHand)
	{
		SecondaryHand = nullptr;
	}
	else if (hand == PrimaryHand )
	{
		if (SecondaryHand != nullptr)
		{
			PrimaryHand->ResetHandMesh();
			PrimaryHand = SecondaryHand;
			PrimaryGrabbedLocation = SecondaryHand->GrabSphereComponent->GetComponentLocation();
			RelativeOffset = PrimaryGrabbedLocation - GetComponentLocation();
			StartOrientation = GetForwardVector();
			SecondaryHand = nullptr;
		}
		else
		{
			PrimaryHand->ResetHandMesh();
			PrimaryHand = nullptr;
			
		}
	}
	

}

bool UDoubleGripStaticMeshComponent::AttachComponent_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	if (SecondaryHand != nullptr)
	{
		IsGrabbed = true;
		PhysicsHandle->GrabComponentAtLocation(this, FName{ "None" }, GrabLocation);
		return true;
	}
	return false;
}

bool UDoubleGripStaticMeshComponent::DettachComponent_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	IsGrabbed = false;
	if (SecondaryHand != nullptr)
	{
		SecondaryHand->PhysicsHandleComponent->ReleaseComponent();
	}
	if (PrimaryHand != nullptr)
	{
		PrimaryHand->PhysicsHandleComponent->ReleaseComponent();
	}
	SetPhysicsLinearVelocity(FVector{});
	return true;
}

bool UDoubleGripStaticMeshComponent::AttachActor_Implementation(USceneComponent* MotionController,
	USceneComponent* ToAttachActor, UPrimitiveComponent* ToSimulateComponent)
{
	if (SecondaryHand != nullptr)
	{
		IsGrabbed = true;
		ToSimulateComponent->SetSimulatePhysics(false);
		ToAttachActor->AttachToComponent(MotionController, FAttachmentTransformRules{ EAttachmentRule::KeepWorld,false });
		return true;
	}
	else return false;
}

bool UDoubleGripStaticMeshComponent::DettachActor_Implementation(USceneComponent* ToDettachActor,
	UPrimitiveComponent* ToSimulateComponent, bool IsSimPhysics)
{
	IsGrabbed = false;
	ToSimulateComponent->SetSimulatePhysics(IsSimPhysics);
	ToDettachActor->DetachFromComponent(FDetachmentTransformRules{ EDetachmentRule::KeepWorld,false });
	return true;
}

bool UDoubleGripStaticMeshComponent::HasGrabbed(bool bothHands)
{
	if (PrimaryHand != nullptr && SecondaryHand != nullptr && IsGrabbed)
	{
		if (bothHands) return true;
		else return false;
	}
	else if (PrimaryHand != nullptr && bothHands == false)
	{
		return true;
	}
	return false;
}

void UDoubleGripStaticMeshComponent::UpdateGrip(float deltaSeconds)
{
	if (PrimaryHand != nullptr)
	{
		FVector rotAxis = FVector::CrossProduct(  StartOrientation, GetForwardVector());
		FVector temp = RelativeOffset;
		if (rotAxis.Normalize())
		{
			//angle between
			FVector v1 = GetForwardVector();
			FVector v2 = StartOrientation;
			float angle = FVector::DotProduct(v1, v2);
			angle = acos(angle) * 180 / PI;
			//rotate
			temp = temp.RotateAngleAxis(angle, rotAxis);
			UE_LOG(LogTemp, Warning, TEXT("Angle: %f Axis: %s V1: %s V2: %s"), angle, *rotAxis.ToString(), *v1.ToString(), *v2.ToString());
		}
		//Relative movement
		PrimaryGrabbedLocation = GetComponentLocation() + temp;
	

		// Effect of resistence 
		FVector offset =PrimaryHand->GrabSphereComponent->GetComponentLocation() - PrimaryGrabbedLocation;
		offset *= VisualResistencePercent;
		PrimaryHand->HandMeshComponent->SetWorldLocation(PrimaryGrabbedLocation + offset);

		
		
	}
}
