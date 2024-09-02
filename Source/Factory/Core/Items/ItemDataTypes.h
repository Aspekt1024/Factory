#pragma once

#include "CoreMinimal.h"
#include "ItemDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	UTexture* Icon = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Items")
	int MaxStackSize = 100;
	
};
