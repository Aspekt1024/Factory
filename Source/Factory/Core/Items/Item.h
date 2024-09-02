#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FACTORY_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Items")
	FDataTableRowHandle ItemData;
	
};
