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

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Procedural Settings")
		void GenerateProceduralTilemap();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Procedural Settings")
		bool GenerateProceduralRoom(int x, int y, Directions d, bool init);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Procedural Settings")
		bool GenerateProceduralFeature();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Procedural Settings")
		bool GenerateFeature(int x, int y, Directions d);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Procedural Settings")
		bool GenerateProceduralCorridor(int x, int y, Directions d);

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
		int32 GetTileByCoordinates(FCoord coordinates);

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
		bool CreateRoom(int x, int y, int new_width, int new_height, TEnumAsByte<TileType> tile_type);

	UFUNCTION()
		void CreateRoomLights();

	UFUNCTION()
		void DeleteTileMap();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		int dungeon_width = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		int dungeon_height = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		int maxFeatures = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		int roomChance = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		int gateChance = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		TArray<FProceduralRoom> rooms;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Procedural Settings")
		TArray<FProceduralRoom> exits;

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

	int proceduralWidth = 30;
	int proceduralHeight = 30;
	bool proceduralCreation = false;
	int maxProceduralFeatures = 30;

	const int maxRoomSize = 6;
	const int minRoomSize = 3;

	const int maxCorridorSize = 6;
	const int minCorridorSize = 3;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
