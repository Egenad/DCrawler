// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnSystemComponent.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTurnSystemComponent::UTurnSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurnSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get all enemies of current level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), enemies);
	
}


// Called every frame
void UTurnSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Every action of the enemies will be made AFTER the INPUT of the player (not the action perself).
void UTurnSystemComponent::processActions() {

	APlayerPawn *player = static_cast<APlayerPawn*>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player != nullptr && player->IsValidLowLevel() && enemies.Num() > 0) {
		for (AActor* actor : enemies) {
			ABaseEnemy* enemy = static_cast<ABaseEnemy*>(actor);
			if (enemy != nullptr && enemy->IsValidLowLevel() && enemy->action_type == T_MOVE) {

			}
		}
	}
}

