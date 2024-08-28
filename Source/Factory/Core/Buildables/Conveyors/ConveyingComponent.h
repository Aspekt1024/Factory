#pragma once

#include "CoreMinimal.h"
#include "Factory/Core/Items/Item.h"
#include "ConveyingComponent.generated.h"

struct FPathingItem;
class UBuildableSplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSplineDelegate, const AItem*, Item); 

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UConveyingComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UConveyingComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyors")
	float ConveyorSpeed;
	
	UPROPERTY(BlueprintAssignable, Category = "Conveyors")
	FItemSplineDelegate OnItemReachedSplineEnd;
	
	UFUNCTION(BlueprintCallable, Category = "Conveyors")
	void Init(UBuildableSplineComponent* SplineComponentRef);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Conveyors")
	void PathItemAlongSpline(AItem* Item);

	UPROPERTY()
	TArray<FPathingItem> PathingItems;
	

private:
	UPROPERTY()
	UBuildableSplineComponent* SplineComponent;
};
