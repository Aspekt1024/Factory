#include "ItemProductionComponent.h"

#include "Factory/Core/Items/ItemDataTypes.h"

void UItemProductionComponent::SetRecipe(const FDataTableRowHandle& RecipeRowHandle)
{
	// TODO clear inventory and give to player etc
	AssignedRecipe = *RecipeRowHandle.GetRow<FItemRecipeData>(FString(""));
	ItemData = *AssignedRecipe.ItemRowHandle.GetRow<FItemData>(FString(""));
	Ingredients.Empty();
	ProductionTime = 0.f;
}

bool UItemProductionComponent::TakeItems(int Count)
{
	if (NumProducedItems < Count) return false;
	NumProducedItems -= Count;
	return true;
}

bool UItemProductionComponent::CanGiveItemAsIngredient(const FDataTableRowHandle& ItemRowHandle, int Count)
{
	if (AssignedRecipe.RecipeName.IsEmpty()) return false;

	for (auto Ingredient : AssignedRecipe.Ingredients)
	{
		if (Ingredient.ItemRowHandle == ItemRowHandle)
		{
			if (Ingredients.Contains(ItemRowHandle.RowName))
			{
				// TODO handle case where the item count can be partially added
				if (Count <= ItemData.MaxStackSize - Ingredients[ItemRowHandle.RowName])
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (Count <= ItemData.MaxStackSize)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	if (AssignedRecipe.ItemRowHandle == ItemRowHandle)
	{
		if (Count <= ItemData.MaxStackSize - NumProducedItems)
		{
			return true;
		}
	}
	
	return false;
}

void UItemProductionComponent::GiveItemAsIngredient(const FDataTableRowHandle& ItemRowHandle, const int Count)
{
	for (auto Ingredient : AssignedRecipe.Ingredients)
	{
		if (Ingredient.ItemRowHandle == ItemRowHandle)
		{
			if (Ingredients.Contains(ItemRowHandle.RowName))
			{
				// TODO handle case where the item count can be partially added
				if (Count <= ItemData.MaxStackSize - Ingredients[ItemRowHandle.RowName])
				{
					Ingredients[ItemRowHandle.RowName] += Count;
					return;
				}
				else
				{
					return;
				}
			}
			else if (Count <= ItemData.MaxStackSize)
			{
				Ingredients.Add(ItemRowHandle.RowName, Count);
				return;
			}
			return;
		}
	}
	
	if (AssignedRecipe.ItemRowHandle == ItemRowHandle)
	{
		if (Count <= ItemData.MaxStackSize - NumProducedItems)
		{
			NumProducedItems += Count;
		}
	}
}

void UItemProductionComponent::Update(const float DeltaTime)
{
	if (AssignedRecipe.ItemRowHandle.IsNull()) return;

	bool IsProductionFinishedThisFrame = false;
	if (IsProducing && NumProducedItems < ItemData.MaxStackSize)
	{
		ProductionTime += DeltaTime;
		if (ProductionTime >= AssignedRecipe.BaseCraftSpeed)
		{
			ProductionTime -= AssignedRecipe.BaseCraftSpeed;
			IsProducing = false;
			IsProductionFinishedThisFrame = true;
			NumProducedItems++;
		}
	}

	if (!IsProducing)
	{
		bool HasIngredients = true;
		for (const auto Ingredient : AssignedRecipe.Ingredients)
		{
			if (!Ingredients.Contains(Ingredient.ItemRowHandle.RowName) || Ingredients[Ingredient.ItemRowHandle.RowName] < Ingredient.Quantity)
			{
				HasIngredients = false;
				break;
			}
		}

		if (HasIngredients)
		{
			IsProducing = true;
			for (const auto Ingredient : AssignedRecipe.Ingredients)
			{
				Ingredients[Ingredient.ItemRowHandle.RowName]--;
			}

			if (!IsProductionFinishedThisFrame)
			{
				ProductionTime = 0.0f;
			}
		}
	}
}
