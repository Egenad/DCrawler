// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TileMap.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(SceneComponent);

	character = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character"));
	character->SetupAttachment(SceneComponent);

	life_bar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
	life_bar->SetupAttachment(character);
	life_bar->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Assets/Mesh/BasePlane.BasePlane'"));
	
	if (MeshAsset.Succeeded()) {
		UStaticMesh* Asset = MeshAsset.Object;
		character->SetStaticMesh(Asset);
	}

	// Properties

	max_health = actual_health = 100;
	last_percentage = 1;
	base_damage = 24;
	mana = 100;
	stamina = 100;
}

void ABaseEnemy::setOrientationToPlayerCamera()
{

	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	APawn* player = controller->GetPawn();

	FVector target_location = player->GetActorLocation();
	FVector my_location = this->GetActorLocation();

	FRotator new_rotation = UKismetMathLibrary::FindLookAtRotation(my_location, target_location);

	FRotator actual_rotation = this->GetActorRotation();
	character->SetWorldRotation(FRotator(0, new_rotation.Yaw, 0));

}

void ABaseEnemy::initializeTile(){

	ATileMap* tilemap = Cast<ATileMap>(UGameplayStatics::GetActorOfClass(this, ATileMap::StaticClass()));
	//Check if there's a tilemap in the world
	if (tilemap->IsValidLowLevel()) {

		//Check if we are standing in a tile (with 1 raycast)

		FVector start_location = GetActorLocation();
		FVector end_location = start_location;
		end_location.Z -= 100;

		FHitResult hit_result;
		GetWorld()->LineTraceSingleByChannel(hit_result, start_location, end_location, ECollisionChannel::ECC_Visibility);

		ATile* found_tile = Cast<ATile>(hit_result.GetActor());

		if (found_tile->IsValidLowLevel() && found_tile->can_step_up && !found_tile->reserved) {
			SetActorLocation(found_tile->GetActorLocation());
			current_tile = found_tile;
			current_tile->reserved = true;
			current_tile->standing_actor = this;
			InitializeMinimapRepresentation();
		}
		else Destroy(); // Tile not found or the enemy can't be there
	}
	else Destroy(); // Tile not found

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Find in which tile we are currently at:
	initializeTile();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	setOrientationToPlayerCamera();

}


void ABaseEnemy::InitializeMinimapRepresentation() {
	if (!minimap_representation && minimap_type) {

		FTransform transform = GetActorTransform();
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = this;

		ATileMap* tm = current_tile->tilemap;

		if (tm) {
			float offset = tm->minimap_Z_offset;
			FVector location = transform.GetLocation();
			location.Z += (offset + 10);
			minimap_representation = GetWorld()->SpawnActor<AMinimapTileRepresentation>(minimap_type, location, FRotator(0,0,0), spawn_params);

			FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
			minimap_representation->AttachToActor(this, rules);
		}
	}
}