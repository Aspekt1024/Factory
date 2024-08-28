#include "ConveyingComponent.h"

#include "BuildableSplineComponent.h"
#include "PathingItem.h"
#include "Factory/Debug/DebugUtil.h"

UConveyingComponent::UConveyingComponent(): ConveyorSpeed(0), SplineComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConveyingComponent::Init(UBuildableSplineComponent* SplineComponentRef)
{
	SplineComponent = SplineComponentRef;
}

void UConveyingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SplineComponent == nullptr || SplineComponent->Spline == nullptr) return;

	const float DistTravelled = ConveyorSpeed * DeltaTime;
	const float SplineLength = SplineComponent->Spline->GetSplineLength();
	for (int i = PathingItems.Num() - 1; i >= 0; --i)
	{
		const float NewDist = PathingItems[i].PathDistance + DistTravelled;
		if (NewDist >= SplineLength)
		{
			PathingItems[i].PathDistance = SplineLength;
			const FVector NewLocation = SplineComponent->Spline->GetWorldLocationAtDistanceAlongSpline(SplineLength);
			PathingItems[i].Item->SetActorLocation(NewLocation);
			AItem* Item = PathingItems[i].Item;
			PathingItems.RemoveAt(i);
			//Position needs to be set before broadcast
			OnItemReachedSplineEnd.Broadcast(Item);
		}
		else
		{
			PathingItems[i].PathDistance = NewDist;
			const FVector NewLocation = SplineComponent->Spline->GetWorldLocationAtDistanceAlongSpline(NewDist);
			PathingItems[i].Item->SetActorLocation(NewLocation);
		}
	}
}

void UConveyingComponent::PathItemAlongSpline(AItem* Item)
{
	PathingItems.Add(FPathingItem(Item, 0.f));
}
