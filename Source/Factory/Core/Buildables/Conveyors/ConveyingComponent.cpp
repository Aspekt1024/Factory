#include "ConveyingComponent.h"

#include "BuildableSplineComponent.h"
#include "PathingItem.h"

UConveyingComponent::UConveyingComponent(): ConveyorSpeed(0), ItemSeparationDist(0), SplineComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConveyingComponent::Init(UBuildableSplineComponent* SplineComponentRef)
{
	SplineComponent = SplineComponentRef;
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

void UConveyingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SplineComponent == nullptr || SplineComponent->Spline == nullptr) return;

	const float DistTravelled = ConveyorSpeed * DeltaTime;
	const float SplineLength = SplineComponent->Spline->GetSplineLength();
	float MaxDist = SplineLength;
	bool IsLastItemAtEnd = false;
	for (int i = 0; i < PathingItems.Num(); ++i)
	{
		float NewDist = PathingItems[i].PathDistance + DistTravelled;
		const bool IsAtMaxDist = NewDist >= MaxDist;
		if (IsAtMaxDist)
		{
			NewDist = MaxDist;
		}
		
		PathingItems[i].PathDistance = NewDist;
		const FVector NewLocation = SplineComponent->Spline->GetWorldLocationAtDistanceAlongSpline(NewDist);
		PathingItems[i].Item->SetActorLocation(NewLocation);

		if (IsAtMaxDist && i == 0) // first item is always head of the queue
		{
			IsLastItemAtEnd = true;
		}

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
