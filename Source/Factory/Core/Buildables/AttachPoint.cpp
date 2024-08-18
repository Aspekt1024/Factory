#include "AttachPoint.h"
#include "Buildable.h"
#include "Components/ShapeComponent.h"

void AAttachPoint::Initialise(ABuildable* Parent, UShapeComponent* AttachCollider)
{
	ParentBuildable = Parent;
	Collider = AttachCollider;
}

void AAttachPoint::AttachOther(AAttachPoint* Other)
{
	AttachedOthers.AddUnique(Other);
	Other->AttachedOthers.AddUnique(this);
	ParentBuildable->OnAttachmentAdded(this, Other);
	Other->ParentBuildable->OnAttachmentAdded(Other, this);
}

void AAttachPoint::DetachOther(AAttachPoint* Other)
{
	AttachedOthers.AddUnique(Other);
	Other->ParentBuildable->OnAllAttachmentsRemoved(this);
}

void AAttachPoint::SetConnectionEnabled(const bool IsEnabled) const
{
	if (IsEnabled)
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	}
	else
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}
