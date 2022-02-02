// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tile.h"
#include "Utility.h"

#include "TileMap.generated.h"

UCLASS()
class DCRAWLER_API ATileMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileMap();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Settings")
		void GenerateTilemap();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Reset")
		void ResetTilemap();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Reset")
		void RecalculeWidthHeight();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Settings")
		void EnlargeTilemapX();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Settings")
		void EnlargeTilemapMX();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Settings")
		void EnlargeTilemapY();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Settings")
		void EnlargeTilemapMY();

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		FCoord TransformWorldLocationToCoordinate(FVector location);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		int32 GetTileByLocation(FCoord coordinates);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		FCoord GetLocationByTile(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		TSubclassOf<ATile> GetTileClassByType(TEnumAsByte<TileType> tile_type);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		ATile* GenerateTileType(FCoord coordinates, FTransform transform, TEnumAsByte<TileType> tile_type);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		void TeleportPlayerToTile(FCoord coordinates);

	UFUNCTION()
		void SetTileNeighbours(ATile* tile);

	UFUNCTION()
		void TeleportPlayerToStart();

	UFUNCTION()
		TEnumAsByte<Directions> GetDirectionByRotation(FRotator rotation, bool &success);

	UFUNCTION(BlueprintCallable, Category = "Player")
		void InitPlayerRotation(FRotator new_rotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		TArray<ATile*> tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		TMap<TEnumAsByte<TileType>, TSubclassOf<ATile>> type_map;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tilemap")
		TMap<TEnumAsByte<Directions>, FDirectionData> direction_map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		FDirectionData player_start {FCoord{0, 0}, FRotator(0.0,0.0,0.0)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int default_width = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int default_height = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		int minimap_Z_offset = -1500;

	// Getters

	UFUNCTION(BlueprintCallable, Category = "Get")
		int GetTilemapWidth();

	UFUNCTION(BlueprintCallable, Category = "Get")
		int GetTileSize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	int width = 5;
	int height = 5;
	int TileSize = 400;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
