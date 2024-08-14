// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactoryGameMode.h"
#include "FactoryCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFactoryGameMode::AFactoryGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
