// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAbility.h"
#include "Tile.h"
#include "BaseEnemy.generated.h"

UCLASS()
class DCRAWLER_API ABaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		int level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float base_damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		TArray<TSubclassOf<ABaseAbility>> abilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
		ATile* current_tile;

	// Comps

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UStaticMeshComponent* character;

	// Functions

	UFUNCTION(BlueprintCallable, Category = "Orientation")
		void setOrientationToPlayerCamera();

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		void initializeTile();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
