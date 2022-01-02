// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "BaseEnemy.h"
#include "TileMap.generated.h"

USTRUCT(BlueprintType)
struct FDirectionData {

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		FCoord direction;

	UPROPERTY(BlueprintReadOnly)
		FRotator rotation;
};

UCLASS()
class DCRAWLER_API ATileMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileMap();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tilemap")
		void GenerateTilemap();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tilemap")
		void ResetTilemap();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tilemap")
		FCoord TransformWorldLocationToCoordinate(FVector location);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		int32 GetTileByLocation(FCoord coordinates);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		FCoord GetLocationByTile(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		TSubclassOf<ATile> GetTileClassByType(TEnumAsByte<TileType> tile_type);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		ATile* GenerateTileType(FTransform transform, TEnumAsByte<TileType> tile_type);

	UFUNCTION(BlueprintCallable, Category = "Tilemap")
		void TeleportPlayerToTile(FCoord coordinates);

	UFUNCTION()
		void SetTileNeighbours(ATile* tile);

	UFUNCTION()
		TEnumAsByte<Directions> GetDirectionByRotation(FRotator rotation, bool &success);

	UFUNCTION(BlueprintCallable, Category = "Player")
		void InitPlayerRotation(FRotator new_rotation);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		int width = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		int TileSize = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		TArray<ATile*> tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		TMap<TEnumAsByte<TileType>, TSubclassOf<ATile>> type_map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TMap<FString, TSubclassOf<ABaseEnemy>> enemy_map;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tilemap")
		TMap<TEnumAsByte<Directions>, FDirectionData> direction_map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilemap")
		FDirectionData player_start {FCoord{0, 0}, FRotator(0.0,0.0,0.0)};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
