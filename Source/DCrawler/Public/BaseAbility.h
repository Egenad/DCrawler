// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAbility.generated.h"

UENUM()
enum ability_category {
	A_WATER			UMETA(DisplayName = "Water"),
	A_EARTH			UMETA(DisplayName = "Earth"),
	A_FIRE			UMETA(DisplayName = "Fire"),
	A_WIND			UMETA(DisplayName = "Wind"),
	A_POISON		UMETA(DisplayName = "Poison")
};

UENUM()
enum ability_type {
	A_PHYSICAL		UMETA(DisplayName = "Physical"),
	A_SPELL			UMETA(DisplayName = "Spell")
};

UCLASS()
class DCRAWLER_API ABaseAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAbility();

	// FUNCTIONS

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void executeAbility();

	// VARIABLES

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float precision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		TEnumAsByte<ability_category> category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		TEnumAsByte<ability_type> type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
