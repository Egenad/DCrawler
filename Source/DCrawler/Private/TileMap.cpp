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

	TeleportPlayerToStart();
	InitPlayerRotation(player_start.rotation);

}

// Called every frame
void ATileMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileMap::GenerateTilemap() {

	if (!type_map.IsEmpty()) {
		
		if (!proceduralCreation) {
			width = default_width;
			height = default_height;
		}
		else {
			width = proceduralWidth;
			height = proceduralHeight;
		}

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

				if (tile_position.c_x == 0 || j == (width - 1) || tile_position.c_y == 0 || i == (height-1) || proceduralCreation) {
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

void ATileMap::GenerateProceduralTilemap() {

	proceduralWidth = dungeon_width;
	proceduralHeight = dungeon_height;
	maxProceduralFeatures = maxFeatures;

	if (proceduralWidth < 30) proceduralWidth = 30;
	if (proceduralHeight < 30) proceduralHeight = 30;

	DeleteTileMap(); // Delete all tiles if any

	if (!type_map.IsEmpty()) {

		// Generate Width x Height tiles of WALL type.
		proceduralCreation = true;
		GenerateTilemap();
		
		// Generate the first room in the middle of the dungeon.
		if (GenerateProceduralRoom(proceduralWidth / 2, proceduralHeight / 2, static_cast<Directions>(FMath::RandRange(0, 3)), true)) {
			for (int i = 0; i < maxProceduralFeatures; i++) {
				if (!GenerateProceduralFeature()) {
					UE_LOG(LogTemp, Warning, TEXT("Can't create more features. Created: %i"), i);
					break;
				}
			}

			// Generate light points for every room
			CreateRoomLights();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Couldn't create the first room."));
		}
	}

	proceduralCreation = false;
}

void ATileMap::CreateRoomLights() {

	for (int i = 0; i < rooms.Num(); i++) {
		if (rooms[i].room_type == T_ROOM) {
			// We have to take into consideration the width and height of the room. Maximum lights per room are also 4, being 1 the minimum.

			int minRoomTotalSize = minRoomSize * minRoomSize;
			int maxRoomTotalSize = maxRoomSize * maxRoomSize;

			int secondInterval = (2 * maxRoomTotalSize) / 4;
			int thirdInterval = (3 * maxRoomTotalSize) / 4;

			int maxActualRoomSize = rooms[i].height * rooms[i].width;

			int lightsNumber = FMath::RandRange(0, maxActualRoomSize);
			int finalNumber = 1;

			if (lightsNumber < secondInterval && lightsNumber >= minRoomTotalSize) { // 2 Lights
				finalNumber = 2;
			}
			else if (lightsNumber < thirdInterval && lightsNumber >= secondInterval) { // 3 Lights
				finalNumber = 3;
			}
			else if (lightsNumber >= thirdInterval) { // 4 Lights
				finalNumber = 4;
			}

			TArray<FCoord> corners;

			int x = rooms[i].coordinates.c_x;
			int y = rooms[i].coordinates.c_y;

			corners.Add(FCoord{ x, y });
			corners.Add(FCoord{ x + rooms[i].width - 1, y });
			corners.Add(FCoord{ x, y + rooms[i].height - 1 });
			corners.Add(FCoord{ x + rooms[i].width - 1, y + rooms[i].height - 1 });

			for (int j = 0; j < finalNumber; j++) {
				// We have to create the point lights in each corner of the room.
				// The first corner would be {x,y}, and the last one {x+width-1, y+height-1}
				// We can randomize which corner gets the light point (this won't have effect in cases of 4 lights)

				int cornerIndex = FMath::RandRange(0, corners.Num() - 1);

				int corner_x = corners[cornerIndex].c_x;
				int corner_y = corners[cornerIndex].c_y;

				//Now for this corner we need to check if there are 2 Floor tile types and 2 Wall tile types in the neighbours 
				// (otherwise it could be a tile connector between room/corridor - corridor/room)

				int tileIndex = GetTileByCoordinates(corners[cornerIndex]);

				if (tiles[tileIndex] != nullptr && tiles[tileIndex]->type == TT_Terrain) {
					//Get the neighbours from this tile and count the tile types.
					TMap<TEnumAsByte<Directions>, ATile*> neighbours = tiles[tileIndex]->neighbours;

					int wallTypes = 0;
					int floorTypes = 0;

					for (const TPair<TEnumAsByte<Directions>, ATile*> pair : neighbours) {
						if (pair.Value->type == TT_Wall) {
							wallTypes++;
						}
						else if (pair.Value->type == TT_Terrain) {
							floorTypes++;
						}
					}

					if (floorTypes == 2 && wallTypes == 2) {
						tiles[tileIndex]->type = TT_Lamp;
						tiles[tileIndex]->ChangeTileTypeByType();
					}
				}

				corners.RemoveAt(cornerIndex);
			}
		}
	}
}

bool ATileMap::GenerateProceduralRoom(int x, int y, Directions d, bool init) {

	int room_width = FMath::RandRange(minRoomSize, maxRoomSize);
	int room_height = FMath::RandRange(minRoomSize, maxRoomSize);

	int xRoom = x;
	int yRoom = y;

	switch (d) {
	case D_DOWN:	// SOUTH
		xRoom = x - room_width / 2;
		yRoom = y + 1;
		break;
	case D_UP:		// NORTH
		xRoom = x - room_width / 2;
		yRoom = y - room_height;
		break;
	case D_LEFT:	// WEST
		xRoom = x - room_width;
		yRoom = y - room_height / 2;
		break;
	case D_RIGHT:	// EAST
		xRoom = x + 1;
		yRoom = y - room_height / 2;
		break;
	}

	if (CreateRoom(xRoom, yRoom, room_width, room_height, TT_Terrain)) {

		// Save the room itself

		FProceduralRoom first_room{ FCoord{xRoom,yRoom}, room_width, room_height, T_ROOM };
		rooms.Add(first_room);

		// Save possible exits of the room

		if(d != D_DOWN || init) // North border
			exits.Add(FProceduralRoom{ FCoord{xRoom, yRoom - 1}, room_width, 1, T_ROOM });
		if(d != D_UP || init) // South border
			exits.Add(FProceduralRoom{ FCoord{xRoom, yRoom + room_height}, room_width, 1, T_ROOM });
		if (d != D_RIGHT || init) //West border
			exits.Add(FProceduralRoom{ FCoord{xRoom - 1, yRoom}, 1, room_height, T_ROOM });
		if (d != D_LEFT || init) //East border
			exits.Add(FProceduralRoom{ FCoord{xRoom + room_width, yRoom}, 1, room_height, T_ROOM });

		return true;
	}

	return false;
}

bool ATileMap::CreateRoom(int x, int y, int new_width, int new_height, TEnumAsByte<TileType> tile_type) {

	// Check if the room can be created --> Condition: all tiles are of type WALL

	if (x < 1 || y < 1 || x + new_width > proceduralWidth - 1 || y + new_height > proceduralHeight - 1)
		return false;

	for(int i = x - 1; i < x + new_width + 1; i++)
		for (int j = y - 1; j < y + new_height + 1; j++) {
			int tile_index = GetTileByCoordinates(FCoord{ i, j });

			if (tile_index == -1 || tiles[tile_index] == nullptr || tiles[tile_index]->type != TT_Wall) {
				return false;
			}
		}

	// We can create the room

	for (int i = x - 1; i < x + new_width + 1; i++)
		for (int j = y - 1; j < y + new_height + 1; j++) {
			int tile_index = GetTileByCoordinates(FCoord{ i, j });
			if (tile_index != -1 && tiles[tile_index] != nullptr) {
				ATile* tile = tiles[tile_index];
				if (i != x - 1 && j != y - 1 && i != x + new_width && j != y + new_height) {
					tile->type = tile_type;
				}
				tile->ChangeTileTypeByType();
			}
		}

	return true;
}

bool ATileMap::GenerateProceduralFeature() {

	// 'Feature' means a new room or a new corridor.

	for (int i = 0; i < 1000; i++) {
		if (exits.IsEmpty())
			break;

		int random_room_index = FMath::RandRange(0, exits.Num() - 1);
		int x = FMath::RandRange(exits[random_room_index].coordinates.c_x, exits[random_room_index].coordinates.c_x + exits[random_room_index].width - 1);
		int y = FMath::RandRange(exits[random_room_index].coordinates.c_y, exits[random_room_index].coordinates.c_y + exits[random_room_index].height - 1);

		// Try to create a feature in every direction till we get one.
		for (int j = 0; j < 4; j++) {
			if (GenerateFeature(x, y, static_cast<Directions>(j))) {
				// If we could create a feature, erase the exit from the array.
				exits.RemoveAt(random_room_index);
				return true;
			}
		}
	}

	return false;
}

bool ATileMap::GenerateFeature(int x, int y, Directions d) {

	int direction_x = 0;
	int direction_y = 0;

	switch (d) {
	case D_UP: // North
		direction_y = 1;
		break;
	case D_DOWN: // South
		direction_y = -1;
		break;
	case D_LEFT: // West
		direction_x = 1;
		break;
	case D_RIGHT: // East
		direction_x = -1;
		break;
	}

	int tile_index = GetTileByCoordinates(FCoord{ x + direction_x, y + direction_y });
	if (tile_index != -1 && tiles[tile_index] != nullptr && tiles[tile_index]->type == TT_Terrain) {
		if (FMath::RandRange(0, 99) < roomChance) {
			// Create a new room
			if (GenerateProceduralRoom(x, y, d, false)) {
				int new_tile_index = GetTileByCoordinates(FCoord{ x, y });
				if (new_tile_index != -1 && tiles[tile_index] != nullptr) {
					if (FMath::RandRange(0, 99) < gateChance) {
						tiles[new_tile_index]->type = TT_Gate;
					}
					else {
						tiles[new_tile_index]->type = TT_Terrain;
					}
					tiles[new_tile_index]->ChangeTileTypeByType();
				}
				return true;
			}
		}
		else {
			// Create a new corridor
			if (GenerateProceduralCorridor(x, y, d)) {
				int new_tile_index = GetTileByCoordinates(FCoord{ x, y });
				if (new_tile_index != -1 && tiles[tile_index] != nullptr) {
					tiles[new_tile_index]->type = TT_Terrain;
					tiles[new_tile_index]->ChangeTileTypeByType();
				}
				return true;
			}
		}
	}

	return false;
}

bool ATileMap::GenerateProceduralCorridor(int x, int y, Directions d) {

	FProceduralRoom corridor{ FCoord{x, y}, 0, 0, T_CORRIDOR };

	if (FMath::RandBool()) { // Horizontal
		corridor.width = FMath::RandRange(minCorridorSize, maxCorridorSize);
		corridor.height = 1;
		corridor.coordinates = FCoord{ x, y };

		switch (d) {
		case D_UP:
			corridor.coordinates.c_y = y - 1;
			if (FMath::RandBool()) { // West, otherwise east. Have to add 1 to match de north entrance.
				corridor.coordinates.c_x = x - corridor.width + 1;
			}
			break;
		case D_DOWN:
			corridor.coordinates.c_y = y + 1;
			if (FMath::RandBool()) { // West, otherwise east. Have to add 1 to match the south entrance.
				corridor.coordinates.c_x = x - corridor.width + 1;
			}
			break;
		case D_LEFT: // West
			corridor.coordinates.c_x = x - corridor.width;
			break;
		case D_RIGHT: // East
			corridor.coordinates.c_x = x + 1;
			break;
		}
	}
	else { // Vertical
		corridor.width = 1;
		corridor.height = FMath::RandRange(minCorridorSize, maxCorridorSize);

		switch (d) {
		case D_UP:
			corridor.coordinates.c_y = y - corridor.height;
			break;
		case D_DOWN:
			corridor.coordinates.c_y = y + 1;
			break;
		case D_LEFT: // West

			corridor.coordinates.c_x = x - 1;

			if (FMath::RandBool()) { // North
				corridor.coordinates.c_y = y - corridor.height + 1;
			}
			break;
		case D_RIGHT: // East
			
			corridor.coordinates.c_x = x + 1;

			if (FMath::RandBool()) { // North
				corridor.coordinates.c_y = y - corridor.height + 1;
			}
			break;
		}
	}

	if (CreateRoom(corridor.coordinates.c_x, corridor.coordinates.c_y, corridor.width, corridor.height, TT_Terrain)) {
		// Save the room itself

		rooms.Add(corridor);

		// Save possible exits of the room

		if (d != D_DOWN && corridor.height != 1) // North border
			exits.Add(FProceduralRoom{ FCoord{corridor.coordinates.c_x, corridor.coordinates.c_y - 1}, corridor.width, 1, T_CORRIDOR });
		if (d != D_UP && corridor.height != 1) // South border
			exits.Add(FProceduralRoom{ FCoord{corridor.coordinates.c_x, corridor.coordinates.c_y + corridor.height}, corridor.width, 1, T_CORRIDOR });
		if (d != D_LEFT && corridor.height != 1) //East border
			exits.Add(FProceduralRoom{ FCoord{corridor.coordinates.c_x + corridor.width, corridor.coordinates.c_y}, 1, corridor.height, T_CORRIDOR });
		if (d != D_RIGHT && corridor.height != 1) //West border
			exits.Add(FProceduralRoom{ FCoord{corridor.coordinates.c_x - 1, corridor.coordinates.c_y}, 1, corridor.height, T_CORRIDOR });

		return true;
	}

	return false;
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

int32 ATileMap::GetTileByCoordinates(FCoord coordinates) {

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

	DeleteTileMap();
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

			int tile_index = GetTileByCoordinates(FCoord{ aux_x, aux_y });

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

					int index = GetTileByCoordinates(FCoord{ j, i });
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

					int index = GetTileByCoordinates(FCoord{ j, i });
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

					int index = GetTileByCoordinates(FCoord{ (j - 1), i });
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

					int index = GetTileByCoordinates(FCoord{ j, i-1 });
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

void ATileMap::TeleportPlayerToStart() {
	APlayerPawn* player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player != nullptr) {
		//Check if there's a tile marked as starting point
		
		bool teleported = false;

		for (int i = 0; i < tiles.Num() && !teleported; i++) {
			if (tiles[i]) {
				if (tiles[i]->start_tile) {
					TeleportPlayerToTile(tiles[i]->coordinates);
					teleported = true;
				}
			}
		}

		if (!teleported) {
			TeleportPlayerToTile(player_start.direction);
		}
	}
}

void ATileMap::TeleportPlayerToTile(FCoord coordinates) {
	
	APlayerPawn* player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player != nullptr) {

		ATile* tile = nullptr;
		int index = GetTileByCoordinates(coordinates);

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
				if (tiles[i]) {
					if (tiles[i]->can_start && tiles[i]->coordinates.c_x == coordinates.c_x && tiles[i]->coordinates.c_y == coordinates.c_y) {
						found = true;
						tile = tiles[i];
					}
				}
			}

			// If we couldn't find that tile, get the first abailable in the map.
			if (!found) {
				for (int i = 0; i < tiles.Num() && !found; i++) {
					if (tiles[i]) {
						if (tiles[i]->can_start) {
							found = true;
							tile = tiles[i];
						}
					}
				}
			}
		}

		tile->reserved = true;
		player->current_tile = tile;
		player->SetActorLocation(tile->GetActorLocation());
		player->actual_location = tile->GetActorLocation();
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

void ATileMap::DeleteTileMap() {
	if (tiles.Num() > 0) {
		for (int i = 0; i < tiles.Num(); i++) {
			if (IsValid(tiles[i])) {

				if (tiles[i]->minimap_representation->IsValidLowLevel())
					tiles[i]->minimap_representation->Destroy();

				if (tiles[i]->Destroy())
					tiles[i] = nullptr;
			}
		}
		tiles.Empty();
	}

	rooms.Empty();
	exits.Empty();
}

int ATileMap::GetTilemapWidth() {
	return width;
}


int ATileMap::GetTileSize() {
	return TileSize;
}