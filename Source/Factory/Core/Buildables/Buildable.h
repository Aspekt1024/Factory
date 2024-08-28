#pragma once

#include "CoreMinimal.h"
#include "Buildable.generated.h"

class AAttachPoint;

UCLASS(Blueprintable, BlueprintType)
class ABuildable : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "AttachPoint")
	void OnAllAttachmentsRemoved(AAttachPoint* AttachPoint);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AttachPoint")
	void OnAttachmentAdded(AAttachPoint* AttachPoint, AAttachPoint* Other);

	UFUNCTION(BlueprintImplementableEvent, Category = "AttachPoint")
	void OnItemReceived(AAttachPoint* AttachPoint, AItem* Item);
};
