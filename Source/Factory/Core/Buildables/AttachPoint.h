#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "AttachPoint.generated.h"

UENUM(BlueprintType)
enum class EAttachDirection : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Input = 1 UMETA(DisplayName = "Input"),
	Output = 2 UMETA(DisplayName = "Output"),
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class FACTORY_API AAttachPoint : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachPoint")
	EAttachDirection Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachPoint")
	ABuildable* AttachedBuildable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachPoint")
	ABuildable* ParentBuildable;
	
};
