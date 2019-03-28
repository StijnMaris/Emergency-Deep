#include "GrippableStaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EDPlayerHand.h"
void UGrippableStaticMeshComponent::OnGrab_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	
}

void UGrippableStaticMeshComponent::OnGrabRelease_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	
}

void UGrippableStaticMeshComponent::OnSideGrip_Implementation(USceneComponent* MotionController,
	UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	
}

bool UGrippableStaticMeshComponent::HasBeenGripped_Implementation() const
{
	return IsGripped;
}

bool UGrippableStaticMeshComponent::AttachComponent_Implementation(USceneComponent* MotionController, UPhysicsHandleComponent* PhysicsHandle, FVector GrabLocation)
{
	if (MotionContr != nullptr)
	{
		//Release
		AEDPlayerHand* hand = Cast<AEDPlayerHand>(MotionContr->GetOwner());
		if (hand != nullptr)
		{
			hand->DropObject();
		}
	}
	MotionContr = MotionController;
	IsGripped = true;
	PhysicsHandle->GrabComponentAtLocation(this, FName{ "None" }, GrabLocation);
	return true;
}

bool UGrippableStaticMeshComponent::DettachComponent_Implementation(UPhysicsHandleComponent* PhysicsHandle)
{
	MotionContr = nullptr;
	IsGripped = false;
	PhysicsHandle->ReleaseComponent();
	SetPhysicsLinearVelocity(FVector{});
	return true;
}

bool UGrippableStaticMeshComponent::AttachActor_Implementation(USceneComponent* MotionController,
	USceneComponent* ToAttachActor, UPrimitiveComponent* ToSimulateComponent)
{
	if (MotionContr != nullptr)
	{
		//Release
		AEDPlayerHand* hand = Cast<AEDPlayerHand>(MotionContr->GetOwner());
		if (hand != nullptr)
		{
			hand->DropObject();
		}
	}
	MotionContr = MotionController;
	IsGripped = true;
	ToSimulateComponent->SetSimulatePhysics(false);
	ToAttachActor->AttachToComponent(MotionController, FAttachmentTransformRules{ EAttachmentRule::KeepWorld,false });
	return true;
}

bool UGrippableStaticMeshComponent::DettachActor_Implementation(USceneComponent* ToDettachActor,
	UPrimitiveComponent* ToSimulateComponent, bool IsSimPhysics)
{
	IsGripped = false;
	ToSimulateComponent->SetSimulatePhysics(IsSimPhysics);
	ToDettachActor->DetachFromComponent(FDetachmentTransformRules{ EDetachmentRule::KeepWorld,false });
	return true;
}


