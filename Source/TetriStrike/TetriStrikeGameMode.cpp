// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriStrikeGameMode.h"
#include "TetriStrikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATetriStrikeGameMode::ATetriStrikeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
