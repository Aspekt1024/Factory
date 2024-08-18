#pragma once

#include "CoreMinimal.h"
#include "Buildable.generated.h"

class AAttachPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttachPointsDelegate, AAttachPoint*, AttachPoint);

UCLASS(Blueprintable, BlueprintType)
class ABuildable : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "AttachPoint")
	void OnAllAttachmentsRemoved(AAttachPoint* AttachPoint);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AttachPoint")
	void OnAttachmentAdded(AAttachPoint* AttachPoint, AAttachPoint* Other);
};
