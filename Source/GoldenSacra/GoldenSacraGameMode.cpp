// Copyright Epic Games, Inc. All Rights Reserved.

#include "GoldenSacraGameMode.h"
#include "GoldenSacraCharacter.h"
#include "Public/CustomGameInstance.h"
#include "Public/Utility.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AGoldenSacraGameMode::AGoldenSacraGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	turnSystemCP = CreateDefaultSubobject<UTurnSystemComponent>(TEXT("Turn System Component"));
	this->AddOwnedComponent(turnSystemCP);

	addGameListeners();
}

void AGoldenSacraGameMode::addGameListeners(){
	if(GetWorld() != nullptr){
		UCustomGameInstance* game_instance = static_cast<UCustomGameInstance*>(UGameplayStatics::GetGameInstance(GetWorld()));
		if(game_instance->IsValidLowLevel()){
			game_instance->addListener(TT_PROCESS_TURN, [&](){turnSystemCP->processActions();});
		}
	}
}
