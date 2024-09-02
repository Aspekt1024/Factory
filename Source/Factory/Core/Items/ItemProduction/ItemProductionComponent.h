#pragma once

#include "CoreMinimal.h"
#include "ItemRecipeData.h"
#include "Factory/Core/Items/ItemDataTypes.h"
#include "ItemProductionComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UItemProductionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FItemRecipeData AssignedRecipe;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, int> Ingredients;

	UPROPERTY(BlueprintReadOnly)
	bool IsProducing;
	
	UPROPERTY(BlueprintReadOnly)
	float ProductionTime;
	
	UPROPERTY(BlueprintReadOnly)
	int NumProducedItems;
	
	UFUNCTION(BlueprintCallable, Category = "Item Production")
	void SetRecipe(const FDataTableRowHandle& RecipeRowHandle);

	UFUNCTION(BlueprintCallable, Category = "ItemProduction")
	bool TakeItems(int Count);

	UFUNCTION(BlueprintCallable, Category = "Item Production")
	bool CanGiveItemAsIngredient(const FDataTableRowHandle& ItemRowHandle, int Count);

	UFUNCTION(BlueprintCallable, Category = "Item Production")
	void GiveItemAsIngredient(const FDataTableRowHandle& ItemRowHandle, int Count);

	UFUNCTION(BlueprintCallable, Category = "Item Production")
	void Update(float DeltaTime);

private:
	UPROPERTY()
	FItemData ItemData;
};
