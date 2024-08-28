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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyors")
	float ItemSeparationDist;
	
	UPROPERTY(BlueprintAssignable, Category = "Conveyors")
	FItemSplineDelegate OnItemReachedSplineEnd;
	
	UFUNCTION(BlueprintCallable, Category = "Conveyors")
	void Init(UBuildableSplineComponent* SplineComponentRef);

	UFUNCTION(BlueprintCallable, Category = "Conveyors")
	bool CanConveyItem() const;
	
	UFUNCTION(BlueprintCallable, Category = "Conveyors")
	void StartConveyingItem(AItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Conveyors")
	void RemoveLastItem();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

private:
	UPROPERTY()
	UBuildableSplineComponent* SplineComponent;

	UPROPERTY()
	TArray<FPathingItem> PathingItems;

	float LastItemDist;
};
