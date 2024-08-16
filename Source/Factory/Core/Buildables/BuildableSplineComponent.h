#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "BuildableSplineComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeshUpdateDelegate, const USplineMeshComponent*, SplineMeshComponent);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UBuildableSplineComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SectionLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StartMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* MidMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* EndMesh;

	UPROPERTY(BlueprintReadOnly)
	TMap<USplineMeshComponent*, FVector2D> SplineMeshComps;
	
	UFUNCTION(BlueprintCallable)
	void InitialiseSpline(USplineComponent* SplineRef, const FTransform& StartPoint, const FTransform& EndPoint);

	UFUNCTION(BlueprintCallable)
	void UpdateSpline(const FTransform& EndPoint);

	UFUNCTION(BlueprintCallable)
	void ClearSpline();

	UPROPERTY(BlueprintAssignable)
	FMeshUpdateDelegate OnMeshComponentCreated;

private:
	UPROPERTY()
	USplineComponent* Spline;

	UPROPERTY()
	FTransform StartTransform;
	
	UPROPERTY()
	FTransform EndTransform;
	
	void SetSplineEndpoint(const int32 PointIndex, const int32 ExtrudeIndex, const FTransform& EndPoint) const;
	void SetExtrudeTangents() const;
	void UpdateMesh();
	void SetStartAndEndPoints(USplineMeshComponent* SplineMeshComponent, const int32 Index, const float TempSectionLength) const;
	USplineMeshComponent* CreateNewSplineMeshComponent() const;
};
