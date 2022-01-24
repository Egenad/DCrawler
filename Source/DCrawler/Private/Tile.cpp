// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "TileMap.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(SceneComponent);
}

void ATile::ChangeTileType(){

	//Spawn a new tile in my exact location of desired new type

	FVector location_to_spawn = this->GetActorLocation();

	FActorSpawnParameters spawn_params;
	TSubclassOf<ATile> new_type = tilemap->GetTileClassByType(type);

	if (new_type != nullptr) {

		ATile* new_tile = GetWorld()->SpawnActor<ATile>(new_type, location_to_spawn, FRotator(0, 0, 0), spawn_params);

		if (new_tile != nullptr) {
			new_tile->tilemap = tilemap;
			new_tile->neighbours = neighbours;

			//Set this tile to the neighbours

			TEnumAsByte<Directions> direction;

			for (int i = 0; i < D_END; i++) {
				
				direction = static_cast<Directions>(i);
				ATile** tile_neighbour = neighbours.Find(direction);

				if (tile_neighbour) {

					ATile* tile = *tile_neighbour;

					int inverse = i + 2;

					if (inverse >= D_END) {
						inverse -= D_END;
					}

					TEnumAsByte<Directions> inverse_direction = static_cast<Directions>(inverse);

					tile->neighbours.Add(inverse_direction, new_tile);
				}
			}

			new_tile->coordinates = coordinates;
			new_tile->type = type;

			// Search in the tilemap the actual tile and replace it with the new one

			int index = tilemap->GetTileByLocation(coordinates);

			if (index != -1) {
				tilemap->tiles[index] = new_tile;
			}

			// Lastly, destroy this actor
			this->Destroy();
		}
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

