// Copyright Epic Games, Inc. All Rights Reserved.

#include "GoldenSacraGameMode.h"
#include "GoldenSacraCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGoldenSacraGameMode::AGoldenSacraGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
