#pragma once

#include "CoreMinimal.h"
#include "Factory/Core/Items/Item.h"
#include "AttachPoint.generated.h"

class ABuildable;

UENUM(BlueprintType)
enum class EAttachDirection : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Input = 1 UMETA(DisplayName = "Input"),
	Output = 2 UMETA(DisplayName = "Output"),
};

UENUM(BlueprintType)
enum class EAttachType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Input = 1 UMETA(DisplayName = "Conveyor"),
	Output = 2 UMETA(DisplayName = "Pipe"),
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class FACTORY_API AAttachPoint : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachPoint")
	EAttachDirection Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachPoint")
	EAttachType AttachmentType;

	UPROPERTY(BlueprintReadOnly, Category = "AttachPoint")
	TArray<AAttachPoint*> AttachedOthers;

	UPROPERTY(BlueprintReadOnly, Category = "AttachPoint")
	ABuildable* ParentBuildable;

	UPROPERTY(BlueprintReadOnly, Category = "AttachPoint")
	AAttachPoint* NextAttachPoint;

	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	void Initialise(ABuildable* Parent, UShapeComponent* AttachCollider);

	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	void SetDirection(EAttachDirection NewDirection);

	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	bool CanAttach(AAttachPoint* Other);
	
	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	void AttachOther(AAttachPoint* Other);

	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	void DetachOther(AAttachPoint* Other);

	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	void SetConnectionEnabled(bool IsEnabled) const;

	UFUNCTION(BlueprintCallable, Category = "AttachPoint")
	void GiveItem(AItem* Item);


private:
	UPROPERTY()
	UShapeComponent* Collider;
	
};
