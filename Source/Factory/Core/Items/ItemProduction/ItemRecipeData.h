#pragma once

#include "CoreMinimal.h"
#include "ItemRecipeData.generated.h"

USTRUCT(BlueprintType)
struct FIngredientData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe", meta=(RowType="ItemData"))
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe")
	int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct FItemRecipeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe")
	FText RecipeName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe", meta=(RowType="ItemData"))
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe")
	TArray<FIngredientData> Ingredients;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe")
	float BaseCraftSpeed = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Recipe")
	int ProductionQuantity = 1;
};
