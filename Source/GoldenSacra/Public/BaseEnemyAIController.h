// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GOLDENSACRA_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	// Functions

	UFUNCTION(BlueprintNativeEvent, Category = "Turn")
		void setTurn(bool turn);

	UFUNCTION(BlueprintNativeEvent, Category = "Turn")
		bool isMyTurn();

	// Properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		bool myTurn;
	
};
