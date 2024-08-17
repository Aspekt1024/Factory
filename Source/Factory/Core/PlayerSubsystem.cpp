#include "PlayerSubsystem.h"

#include "Camera/CameraComponent.h"
#include "Factory/Debug/DebugUtil.h"

void UPlayerSubsystem::InitializeSubsystem(AFactoryCharacter* PlayerRef)
{
	Player = PlayerRef;
	if (Player == nullptr)
	{
		DebugLogError("Failed to find player");
		return;
	}
	DebugLog("Player Subsystem intitialised");
}

void UPlayerSubsystem::LineTraceFromCamera(ECollisionChannel TraceChannel, FHitResult &HitResult_Out, bool &IsHit_Out) const
{
	const UCameraComponent* Cam = Player->GetFirstPersonCameraComponent();
	const FVector Location = Cam->GetComponentLocation();
	const FVector Forward = Cam->GetForwardVector();

	IsHit_Out = GetWorld()->LineTraceSingleByChannel(HitResult_Out, Location, Location + Forward * 1500.f, TraceChannel);
}

float UPlayerSubsystem::GetLookHeightAboveObject(AActor* Actor) const
{
	const FVector DistVector = Player->GetActorLocation() - Actor->GetActorLocation();
	const float HorizontalDist = FMath::Sqrt(FMath::Square(DistVector.X) + FMath::Square(DistVector.Y));
	
	const float Pitch = Player->GetFirstPersonCameraComponent()->GetComponentRotation().Pitch;
	const float VerticalLookDist = DistVector.Z + HorizontalDist * FMath::Tan(FMath::DegreesToRadians(Pitch));
	
	return VerticalLookDist;
}

