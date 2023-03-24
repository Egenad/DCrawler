// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAbility.h"
#include "Tile.h"
#include "MinimapTileRepresentation.h"
#include "Components/WidgetComponent.h"
#include "BaseEnemy.generated.h"

UCLASS()
class GOLDENSACRA_API ABaseEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		int level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
		float last_percentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float actual_health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float max_health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float base_damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float luck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		TArray<TSubclassOf<ABaseAbility>> abilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
		ATile* current_tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinimapRepresentation")
		AMinimapTileRepresentation* minimap_representation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinimapRepresentation")
		TSubclassOf<AMinimapTileRepresentation> minimap_type;

	// Comps

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UStaticMeshComponent* character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UWidgetComponent* life_bar;

	// Functions

	UFUNCTION(BlueprintCallable, Category = "Orientation")
		void setOrientationToPlayerCamera();

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		void initializeTile();

	UFUNCTION(BlueprintCallable, Category = "Minimap")
		void InitializeMinimapRepresentation();

	UFUNCTION(BlueprintCallable, Category = "Attack")
		float GetNextAttackDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
