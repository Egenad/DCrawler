// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Containers/Array.h"
#include "Utility.h"
#include "Interactive.h"
#include "MinimapTileRepresentation.h"

#include "Tile.generated.h"

class ATileMap;

UCLASS()
class GOLDENSACRA_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Functions

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Type")
		void ChangeTileTypeByType();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Type")
		void ChangeTileTypeByClass();

	UFUNCTION(Category = "Type")
		void ChangeTile(TSubclassOf<ATile> new_type);

	UFUNCTION(BlueprintCallable, Category = "Minimap")
		void SeeTile();

	UFUNCTION(BlueprintCallable, Category = "Minimap")
		AMinimapTileRepresentation* CreateMinimapRepresentation();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Start")
		void StartGameHere();

	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
		TEnumAsByte<TileType> type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
		TSubclassOf<ATile> to_type;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Start Signal")
		UStaticMeshComponent* StartSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		FCoord coordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Tile")
		TMap<TEnumAsByte<Directions>, ATile*> neighbours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		bool can_step_up = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		bool can_start = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		bool reserved = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
		bool seen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
		bool start_tile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
		ATileMap* tilemap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactive")
		AInteractive* interactive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
		AMinimapTileRepresentation* minimap_representation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
		TSubclassOf<AMinimapTileRepresentation> minimap_rep_type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
		AActor* standing_actor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
