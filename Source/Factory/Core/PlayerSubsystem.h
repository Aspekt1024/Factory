#pragma once

#include "CoreMinimal.h"
#include "Player/FactoryCharacter.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerSubsystem.generated.h"

UCLASS()
class FACTORY_API UPlayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	AFactoryCharacter* Player;

	UFUNCTION(BlueprintCallable)
	void InitializeSubsystem(AFactoryCharacter* PlayerRef);
	
	UFUNCTION(BlueprintCallable)
	void LineTraceFromCamera(ECollisionChannel TraceChannel, FHitResult &HitResult_Out, bool &IsHit_Out) const;

	UFUNCTION(BlueprintCallable)
	float GetLookHeightAboveObject(AActor* Actor) const;
};
