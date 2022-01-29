// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMap.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"

// Sets default values
ATileMap::ATileMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FDirectionData data;
	
	data.rotation = FRotator(0.0f, 0.0f, -90.f);
	data.direction = FCoord{0,-1};
	direction_map.Add(D_UP, data);

	data.rotation = FRotator(0.f, 0.f, 90.f);
	data.direction = FCoord{0,1};
	direction_map.Add(D_DOWN, data);

	data.rotation = FRotator(0.f, 0.f, 180.f);
	data.direction = FCoord{ -1, 0 };
	direction_map.Add(D_LEFT, data);

	data.rotation = FRotator(0.f, 0.f, 0.f);
	data.direction = FCoord{1,0};
	direction_map.Add(D_RIGHT, data);
}

// Called when the game starts or when spawned
void ATileMap::BeginPlay()
{
	Super::BeginPlay();
	
	//GenerateTilemap();

	FString f = "";
	f.AppendInt(width);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, f);

	TeleportPlayerToTile(player_start.direction);
	InitPlayerRotation(player_start.rotation);

}

// Called every frame
void ATileMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileMap::GenerateTilemap() {

	width = default_width;
	height = default_height;

	if (!type_map.IsEmpty()) {

		FCoord tile_position;
		tile_position.c_x = 0;
		tile_position.c_y = 0;

		//Generate rows
		for (int i = 0; i < height; i++) {
			//Generate columns
			for (int j = 0; j < width; j++) {

				FVector location(tile_position.c_x, tile_position.c_y, 0.0f);
				FRotator rotation(0.0f, 0.0f, 0.0f);

				FTransform transform;
				transform.SetLocation(location);
				transform.SetRotation(rotation.Quaternion());
				transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

				TEnumAsByte<TileType> tile_type = TT_Terrain;

				if (tile_position.c_x == 0 || j == (width - 1) || tile_position.c_y == 0 || i == (height-1)) {
					tile_type = TT_Wall;
				}

				ATile* new_tile = GenerateTileType(FCoord{j,i}, transform, tile_type);

				tiles.Add(new_tile);	//Add new tile to the array

				tile_position.c_x += TileSize;
			}

			tile_position.c_x = 0;
			tile_position.c_y += TileSize;
		}

		// Set tile neighbours

		for (int i = 0; i < tiles.Num(); i++) {
			SetTileNeighbours(tiles[i]);
		}
	}
}

ATile* ATileMap::GenerateTileType(FCoord coordinates, FTransform transform, TEnumAsByte<TileType> tile_type) {

	FActorSpawnParameters spawn_params;
	spawn_params.Owner = this;

	TSubclassOf<ATile> tile_class = GetTileClassByType(tile_type);

	if (tile_class != nullptr) {
		
		// Spawn tile
		ATile* new_tile = GetWorld()->SpawnActor<ATile>(tile_class, transform.GetLocation(), transform.GetRotation().Rotator(), spawn_params);	//Spawn new tile in world
		
		if (new_tile != nullptr) {
			
			// Set properties
			new_tile->coordinates.c_x = coordinates.c_x;
			new_tile->coordinates.c_y = coordinates.c_y;
			new_tile->type = tile_type;
			new_tile->tilemap = this;

			// Spawn its minimap representation
			
			new_tile->CreateMinimapRepresentation();

			// Return the tile ptr
			return new_tile;
		}
	}
	
	return nullptr;
}

int32 ATileMap::GetTileByLocation(FCoord coordinates) {

	int index = ((width * coordinates.c_y) + coordinates.c_x);

	if (index >= 0 && index < (width * height)) {
		return index;
	}

	return -1;
}

FCoord ATileMap::GetLocationByTile(int32 index) {
	
	FCoord aux;

	aux.c_x = index / width;
	aux.c_y = index % width;

	return aux;
}

TSubclassOf<ATile> ATileMap::GetTileClassByType(TEnumAsByte<TileType> tile_type) {

	TSubclassOf<ATile> *result = NULL;

	if (type_map.Contains(tile_type)) {
		result = type_map.Find(tile_type);
	}

	if (result != nullptr) {
		return *result;
	}
	
	return nullptr;
}

void ATileMap::ResetTilemap() {

	if (tiles.Num() > 0) {
		for (int i = 0; i < tiles.Num(); i++) {	
			if (IsValid(tiles[i])) {

				if(tiles[i]->minimap_representation->IsValidLowLevel())
					tiles[i]->minimap_representation->Destroy();

				if(tiles[i]->Destroy())
					tiles[i] = nullptr;
			}
		}
		tiles.Empty();
	}

	GenerateTilemap();
}

FCoord ATileMap::TransformWorldLocationToCoordinate(FVector location){

	int x = location.X;
	int y = location.Y;

	int c_x = round(x / TileSize);
	int c_y = round(y / TileSize);

	return FCoord{ c_x, c_y };
}

void ATileMap::SetTileNeighbours(ATile* tile) {

	int cx = tile->coordinates.c_x;
	int cy = tile->coordinates.c_y;

	//Neighbours

	for (TPair<TEnumAsByte<Directions>, FDirectionData>& direction : direction_map) {
		
		int d_x = direction.Value.direction.c_x;
		int d_y = direction.Value.direction.c_y;

		int aux_x = cx + d_x;
		int aux_y = cy + d_y;

		if (aux_x >= 0 && aux_y >= 0) {

			int tile_index = GetTileByLocation(FCoord{ aux_x, aux_y });

			if (tile_index != -1) {
				if (tiles[tile_index] != nullptr)
					tile->neighbours.Add(direction.Key, tiles[tile_index]);
			}
		}
	}
}

void ATileMap::EnlargeTilemapX() {
	
	if (tiles.Num() > 0) {

		/*
			x = new tile
			o = existing tile
			width = 3

			 0123
			0ooox
			1ooox
			2ooox
		*/

		int new_x = tiles[width-1]->GetActorLocation().X + TileSize;
		int new_y = tiles[width - 1]->GetActorLocation().Y;
	
		TArray<ATile*> new_tiles;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width + 1; j++) {

				if (j != width) {

					// This tile already exists: add it to the new array

					int index = GetTileByLocation(FCoord{ j, i });
					if (index != -1) new_tiles.Add(tiles[index]);
				}
				else {

					// This tile doesn't exists: create it and add it

					FVector location(new_x, new_y, 0.0f);
					FRotator rotation(0.0f, 0.0f, 0.0f);

					FTransform transform;
					transform.SetLocation(location);
					transform.SetRotation(rotation.Quaternion());
					transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

					TEnumAsByte<TileType> tile_type = TT_Terrain;

					ATile* new_tile = GenerateTileType(FCoord{ j,i }, transform, tile_type);
					

					new_tiles.Add(new_tile);

				}
			}
			new_y += TileSize;
		}

		tiles = new_tiles;
		width += 1;

		for (int i = 0; i < new_tiles.Num(); i++) {
			SetTileNeighbours(new_tiles[i]);
		}
	}
}

void ATileMap::EnlargeTilemapY() {

	if (tiles.Num() > 0) {

		/*
			y = new tile
			o = existing tile
			width = 3

			 012
			0ooo
			1ooo
			2ooo
			3yyy
		*/

		int new_y = tiles[(width*height) - 1]->GetActorLocation().Y + TileSize;
		int new_x = tiles[0]->GetActorLocation().X;
		int last_x = new_x;

		TArray<ATile*> new_tiles;

		for (int i = 0; i < height + 1; i++) {
			for (int j = 0; j < width; j++) {

				if (i != height) {

					// This tile already exists: add it to the new array

					int index = GetTileByLocation(FCoord{ j, i });
					if (index != -1) new_tiles.Add(tiles[index]);
				}
				else {

					// This tile doesn't exists: create it and add it

					FVector location(new_x, new_y, 0.0f);
					FRotator rotation(0.0f, 0.0f, 0.0f);

					FTransform transform;
					transform.SetLocation(location);
					transform.SetRotation(rotation.Quaternion());
					transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

					TEnumAsByte<TileType> tile_type = TT_Terrain;

					ATile* new_tile = GenerateTileType(FCoord{ j,i }, transform, tile_type);

					new_tiles.Add(new_tile);

				}
				new_x += TileSize;
			}
			new_x = last_x;
		}

		tiles = new_tiles;
		height += 1;

		for (int i = 0; i < new_tiles.Num(); i++) {
			SetTileNeighbours(new_tiles[i]);
		}
	}
}

void ATileMap::EnlargeTilemapMX() {

	if (tiles.Num() > 0) {

		/*
			x = new tile
			o = existing tile
			width = 3

			 0123
			0xooo
			1xooo
			2xooo
		*/

		TArray<ATile*> new_tiles;

		FCoord tile_position;
		tile_position.c_x = 0;
		tile_position.c_y = 0;

		int new_x = tiles[0]->GetActorLocation().X - TileSize;
		int new_y = tiles[0]->GetActorLocation().Y;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width + 1; j++) {

				if (j != 0) {

					// This tile already exists: add it to the new array

					int index = GetTileByLocation(FCoord{ (j - 1), i });
					if (index != -1) { 
						tiles[index]->coordinates = FCoord{j,i};
						new_tiles.Add(tiles[index]); 
					}
				}
				else {

					// This tile doesn't exists: create it and add it

					FVector location(new_x, new_y, 0.0f);
					FRotator rotation(0.0f, 0.0f, 0.0f);

					FTransform transform;
					transform.SetLocation(location);
					transform.SetRotation(rotation.Quaternion());
					transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

					TEnumAsByte<TileType> tile_type = TT_Terrain;

					ATile* new_tile = GenerateTileType(FCoord{ j,i }, transform, tile_type);

					new_tiles.Add(new_tile);

				}
			}
			new_y += TileSize;
		}

		tiles = new_tiles;
		width += 1;

		for (int i = 0; i < new_tiles.Num(); i++) {
			SetTileNeighbours(new_tiles[i]);
		}
	}
}

void ATileMap::EnlargeTilemapMY() {
	if (tiles.Num() > 0) {

		/*
			y = new tile
			o = existing tile
			width = 3

			 012
			0yyy
			1ooo
			2ooo
			3ooo
		*/

		TArray<ATile*> new_tiles;

		int new_y = tiles[0]->GetActorLocation().Y - TileSize;
		int last_x = tiles[0]->GetActorLocation().X;
		int new_x = last_x;

		for (int i = 0; i < height + 1; i++) {
			for (int j = 0; j < width; j++) {

				if (i != 0) {

					// This tile already exists: add it to the new array

					int index = GetTileByLocation(FCoord{ j, i-1 });
					if (index != -1) {
						tiles[index]->coordinates = FCoord{ j,i };
						new_tiles.Add(tiles[index]);
					}
				}
				else {

					// This tile doesn't exists: create it and add it

					FVector location(new_x, new_y, 0.0f);
					FRotator rotation(0.0f, 0.0f, 0.0f);

					FTransform transform;
					transform.SetLocation(location);
					transform.SetRotation(rotation.Quaternion());
					transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

					TEnumAsByte<TileType> tile_type = TT_Terrain;

					ATile* new_tile = GenerateTileType(FCoord{ j,i }, transform, tile_type);

					new_tiles.Add(new_tile);

				}
				new_x += TileSize;
			}
			new_x = last_x;
		}

		tiles = new_tiles;
		height += 1;

		for (int i = 0; i < new_tiles.Num(); i++) {
			SetTileNeighbours(new_tiles[i]);
		}
	}
}

void ATileMap::TeleportPlayerToTile(FCoord coordinates) {
	
	APlayerPawn* player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player != nullptr) {

		ATile* tile = nullptr;
		int index = GetTileByLocation(coordinates);

		bool valid_tile = false;

		if (index != -1) {
			tile = tiles[index];
			if (tile->can_start) {
				valid_tile = true;
			}
		}
		
		if(!valid_tile){
			//Get first terrain tile of the array;

			bool found = false;

			for (int i = 0; i < tiles.Num() && !found; i++) {
				if (tiles[i]->can_start) {
					found = true;
					tile = tiles[i];
				}
			}
		}

		tile->reserved = true;
		player->current_tile = tile;
		player->SetActorLocation(tile->GetActorLocation());
	}
}

TEnumAsByte<Directions> ATileMap::GetDirectionByRotation(FRotator rotation, bool &success) {

	TEnumAsByte<Directions> result = D_UP;
	success = false;

	float r_z = rotation.Yaw;

	for (TPair<TEnumAsByte<Directions>, FDirectionData>& direction : direction_map) {
		if (direction.Value.rotation.Yaw == r_z) {
			result = direction.Key.GetValue();
			success = true;
		}
	}

	return result;
}

void ATileMap::InitPlayerRotation(FRotator new_rotation) {

	APlayerPawn* player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player != nullptr) {

		bool success = false;
		TEnumAsByte<Directions> new_direction = GetDirectionByRotation(new_rotation, success);

		if (success) {
			player->SetActorRotation(new_rotation);
			player->focused_tile = new_direction;
		}
		else {	// Invalid rotation
			player->SetActorRotation(FRotator(0.0,0.0,-90.0));
			player->focused_tile = D_UP;
		}
	}
}

void ATileMap::RecalculeWidthHeight() {
	if (tiles.Num() > 0) {
		ATile* last_tile = tiles[tiles.Num() - 1];

		if (last_tile) {
			width = last_tile->coordinates.c_x + 1;
			height = last_tile->coordinates.c_y + 1;
		}
	}
}

int ATileMap::GetTilemapWidth() {
	return width;
}


int ATileMap::GetTileSize() {
	return TileSize;
}