// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseEnemy.h"
#include "TurnSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLDENSACRA_API UTurnSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Functions
	UTurnSystemComponent();

	UFUNCTION(BlueprintCallable, Category = "Process")
		void processActions();

	// Properties

	// Array with enemies of current level
	TArray<AActor*> enemies;
		
};
