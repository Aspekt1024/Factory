#include "AttachPoint.h"
#include "Buildable.h"
#include "Components/ShapeComponent.h"

void AAttachPoint::Initialise(ABuildable* Parent, UShapeComponent* AttachCollider)
{
	ParentBuildable = Parent;
	Collider = AttachCollider;
}

void AAttachPoint::SetDirection(EAttachDirection NewDirection)
{
	Direction = NewDirection;
}

bool AAttachPoint::CanAttach(AAttachPoint* Other)
{
	if (Direction != EAttachDirection::None && Other->Direction != EAttachDirection::None)
	{
		return Direction != Other->Direction;
	}
	return true;
}

void AAttachPoint::AttachOther(AAttachPoint* Other)
{
	AttachedOthers.AddUnique(Other);
	Other->AttachedOthers.AddUnique(this);
	ParentBuildable->OnAttachmentAdded(this, Other);
	Other->ParentBuildable->OnAttachmentAdded(Other, this);

	if (Direction != EAttachDirection::None && Other->Direction != EAttachDirection::None)
	{
		NextAttachPoint = Other;
		Other->NextAttachPoint = this;
		SetConnectionEnabled(false);
		Other->SetConnectionEnabled(false);
	}
}

void AAttachPoint::DetachOther(AAttachPoint* Other)
{
	if (Other == NextAttachPoint)
	{
		NextAttachPoint = nullptr;
		SetConnectionEnabled(true);
	}

	if (Other->NextAttachPoint == this)
	{
		Other->NextAttachPoint = nullptr;
		Other->SetConnectionEnabled(true);
	}
	
	AttachedOthers.Remove(Other);
	if (AttachedOthers.Num() == 0)
	{
		ParentBuildable->OnAllAttachmentsRemoved(this);
	}
}

void AAttachPoint::DetachAll()
{
	for (int i = AttachedOthers.Num() - 1; i >= 0; --i)
	{
		AttachedOthers[i]->DetachOther(this);
	}
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

bool AAttachPoint::CanGiveItem(AItem* Item) const
{
	return ParentBuildable->CanReceiveItem(Item);
}

void AAttachPoint::GiveItem(AItem* Item)
{
	ParentBuildable->OnItemReceived(this, Item);
}

float AAttachPoint::GetMinItemDistanceToStart() const
{
	return ParentBuildable->GetMinItemDistanceToStart();
}
