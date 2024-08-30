#include "ConveyingComponent.h"

#include "BuildableSplineComponent.h"
#include "PathingItem.h"

UConveyingComponent::UConveyingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConveyingComponent::Init(UBuildableSplineComponent* SplineComponentRef, AAttachPoint* EndAttachPointRef)
{
	SplineComponent = SplineComponentRef;
	EndAttachPoint = EndAttachPointRef;
	IsInitialised = true;
}

bool UConveyingComponent::CanConveyItem() const
{
	return PathingItems.Num() == 0 || LastItemDist >= ItemSeparationDist;
}

void UConveyingComponent::StartConveyingItem(AItem* Item)
{
	PathingItems.Add(FPathingItem(Item, 0.f));
}

void UConveyingComponent::RemoveLastItem()
{
	if (PathingItems.Num() > 0)
	{
		PathingItems.RemoveAt(0);
	}
}

void UConveyingComponent::DestroyItems()
{
	for (int i = PathingItems.Num() - 1; i >= 0; --i)
	{
		PathingItems[i].Item->Destroy();
	}
	PathingItems.Empty();
}

void UConveyingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsInitialised) return;

	const float DistTravelled = ConveyorSpeed * DeltaTime;
	const float SplineLength = SplineComponent->Spline->GetSplineLength();
	float MaxDist = SplineLength;
	if (EndAttachPoint->NextAttachPoint != nullptr)
	{
		MaxDist -= EndAttachPoint->NextAttachPoint->GetMinItemDistanceToStart();
	}
	else
	{
		MaxDist -= ItemSeparationDist * 0.5f;
	}
	
	bool IsLastItemAtEnd = false;
	for (int i = 0; i < PathingItems.Num(); ++i)
	{
		float NewDist = PathingItems[i].PathDistance + DistTravelled;
		if (i == 0 && NewDist >= SplineLength) // first item is always head of the queue
		{
			IsLastItemAtEnd = true;
		}
		
		if (NewDist >= MaxDist)
		{
			NewDist = MaxDist;
		}
		
		PathingItems[i].PathDistance = NewDist;
		const FVector NewLocation = SplineComponent->Spline->GetWorldLocationAtDistanceAlongSpline(NewDist);
		PathingItems[i].Item->SetActorLocation(NewLocation);

		if (i == PathingItems.Num() - 1)
		{
			LastItemDist = NewDist;
		}

		MaxDist = NewDist - ItemSeparationDist;
	}

	if (IsLastItemAtEnd)
	{
		// Broadcast out of the for loop as we may remove the item from the list
		OnItemReachedSplineEnd.Broadcast(PathingItems[0].Item);
	}
}
