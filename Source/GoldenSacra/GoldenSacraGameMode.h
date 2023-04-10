// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/TurnSystemComponent.h"
#include "GoldenSacraGameMode.generated.h"

UCLASS(minimalapi)
class AGoldenSacraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGoldenSacraGameMode();

	UTurnSystemComponent* turnSystemCP;

private:

	UFUNCTION(BlueprintCallable, Category="Event Manager")
		void addGameListeners();
};



