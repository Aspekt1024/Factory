#include "BuildableSplineComponent.h"

void UBuildableSplineComponent::InitialiseSpline(USplineComponent* SplineRef, const FTransform& StartPoint, const FTransform& EndPoint)
{
	Spline = SplineRef;
	StartTransform = StartPoint;
	EndTransform = EndPoint;

	Spline->ClearSplinePoints(false);
	for (int i = 0; i < 4; i++)
	{
		Spline->AddSplinePointAtIndex(FVector::ZeroVector, i, ESplineCoordinateSpace::World, false);
	}
	
	SetSplineEndpoint(0, 1, StartTransform);
	UpdateSpline(EndTransform);
}

void UBuildableSplineComponent::UpdateSpline(const FTransform& EndPoint)
{
	const int32 EndPointIndex = Spline->GetNumberOfSplinePoints() - 1;
	SetSplineEndpoint(EndPointIndex, EndPointIndex - 1, EndPoint);
	Spline->UpdateSpline();
	UpdateMesh();
}

void UBuildableSplineComponent::ClearSpline()
{
	TArray<USplineMeshComponent*> MeshComps;
	const int32 NumKeys = SplineMeshComps.GetKeys(MeshComps);
	for (int i = NumKeys - 1; i >= 0; --i)
	{
		SplineMeshComps.Remove(MeshComps[i]);
		MeshComps[i]->DestroyComponent();
	}
	Spline->ClearSplinePoints();
}

void UBuildableSplineComponent::SetSplineEndpoint(const int32 PointIndex, const int32 ExtrudeIndex, const FTransform& EndPoint) const
{
	const FVector EndpointLocation = EndPoint.GetLocation();
	const FVector Forward = EndPoint.GetRotation().GetForwardVector() * SectionLength;
	const float Direction = PointIndex < ExtrudeIndex ? 1.f : -1.f;

	Spline->SetLocationAtSplinePoint(PointIndex, EndpointLocation, ESplineCoordinateSpace::World, false);
	Spline->SetLocationAtSplinePoint(ExtrudeIndex, EndpointLocation + Forward * Direction, ESplineCoordinateSpace::World, false);
	
	Spline->SetTangentsAtSplinePoint(PointIndex, Forward, Forward, ESplineCoordinateSpace::World, false);
	Spline->SetTangentsAtSplinePoint(ExtrudeIndex, Forward, Forward, ESplineCoordinateSpace::World, false);
}

void UBuildableSplineComponent::SetExtrudeTangents() const
{
	if (Spline->GetNumberOfSplinePoints() < 4) return;

	constexpr int32 StartExtrudeIndex = 1;
	constexpr int32 EndExtrudeIndex = 2;
	const FVector StartLoc = Spline->GetLocationAtSplinePoint(StartExtrudeIndex, ESplineCoordinateSpace::World);
	const FVector EndLoc = Spline->GetLocationAtSplinePoint(EndExtrudeIndex, ESplineCoordinateSpace::World);

	const FVector DistVector = EndLoc - StartLoc;
	
	Spline->SetTangentsAtSplinePoint(StartExtrudeIndex, DistVector, DistVector, ESplineCoordinateSpace::World, false);
	Spline->SetTangentsAtSplinePoint(EndExtrudeIndex, -DistVector, -DistVector, ESplineCoordinateSpace::World, false);
}

void UBuildableSplineComponent::UpdateMesh()
{
	const int32 NumSections = FMath::Floor(Spline->GetSplineLength() / SectionLength);
	const float TempSectionLength = SectionLength + FMath::Fmod(Spline->GetSplineLength(), SectionLength) / NumSections;
	
	for (int32 Index = 0; Index < NumSections; ++Index)
	{
		USplineMeshComponent* CurrentMeshComp;
		if (Index >= SplineMeshComps.Num())
		{
			CurrentMeshComp = CreateNewSplineMeshComponent();
		}
		else
		{
			TArray<USplineMeshComponent*> MeshComps;
			SplineMeshComps.GetKeys(MeshComps);
			CurrentMeshComp = MeshComps[Index];
		}
	
		const FVector2D Positions = FVector2D(Index * TempSectionLength, (Index + 1) * TempSectionLength);
		SplineMeshComps.Add(CurrentMeshComp, Positions);
		
		SetStartAndEndPoints(CurrentMeshComp, Index, TempSectionLength);
		
		if (Index == 0)
		{
			CurrentMeshComp->SetStaticMesh(StartMesh);
		}
		else if (Index == NumSections - 1)
		{
			CurrentMeshComp->SetStaticMesh(EndMesh);
		}
		else
		{
			CurrentMeshComp->SetStaticMesh(MidMesh);
		}
	}

	TArray<USplineMeshComponent*> MeshComps;
	const int32 NumKeys = SplineMeshComps.GetKeys(MeshComps);
	if (NumKeys > NumSections)
	{
		for (int32 Index = NumKeys - 1; Index >= NumSections; --Index)
		{
			SplineMeshComps.Remove(MeshComps[Index]);
			MeshComps[Index]->DestroyComponent();
		}
	}
}

void UBuildableSplineComponent::SetStartAndEndPoints(USplineMeshComponent* SplineMeshComponent, const int32 Index, const float TempSectionLength) const
{
	const float StartDist = Index * TempSectionLength;
	const float EndDist = (Index + 1) * TempSectionLength;
	
	const FVector StartLoc = Spline->GetLocationAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local);
	const FVector EndLoc = Spline->GetLocationAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local);
	
	const FVector StartTangent = Spline->GetTangentAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local).GetClampedToSize(0.f, SectionLength);
	const FVector EndTangent = Spline->GetTangentAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local).GetClampedToSize(0.f, SectionLength);
	
	SplineMeshComponent->SetStartAndEnd(StartLoc, StartTangent, EndLoc, EndTangent);
}

USplineMeshComponent* UBuildableSplineComponent::CreateNewSplineMeshComponent() const
{
	USplineMeshComponent* MeshComp = NewObject<USplineMeshComponent>(GetOwner(), USplineMeshComponent::StaticClass(), NAME_None, RF_Transient);
	MeshComp->Mobility = EComponentMobility::Type::Movable;
	MeshComp->SetForwardAxis(ESplineMeshAxis::X, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	MeshComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	MeshComp->RegisterComponent();
	GetOwner()->AddInstanceComponent(MeshComp);

	OnMeshComponentCreated.Broadcast(MeshComp);

	return MeshComp;
}

