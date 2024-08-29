#pragma once

#include "CoreMinimal.h"
#include "PathingItem.generated.h"

class AItem;

USTRUCT()
struct FPathingItem
{
	GENERATED_BODY()

	UPROPERTY()
	AItem* Item = nullptr;
		
	float PathDistance = 0.0f;
};
