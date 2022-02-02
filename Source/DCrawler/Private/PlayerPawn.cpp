// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);

	PlayerScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	PlayerScene->SetupAttachment(SceneComponent);

	character = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character"));
	character->SetupAttachment(PlayerScene);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(PlayerScene);

	left_hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand"));
	right_hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand"));

	left_hand->SetupAttachment(character);
	left_hand->SetVisibility(0);
	right_hand->SetupAttachment(character);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Assets/Mesh/BasePlane.BasePlane'"));

	if (MeshAsset.Succeeded()) {
		UStaticMesh* Asset = MeshAsset.Object;
		character->SetStaticMesh(Asset);
		right_hand->SetStaticMesh(Asset);
		left_hand->SetStaticMesh(Asset);
	}
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	InitializeMinimapRepresentation();

	if (TurnCurve) {
		FOnTimelineFloat TurnTimelineProgress;
		TurnTimelineProgress.BindUFunction(this, FName("TurnTimelineProgress"));
		FOnTimelineEvent TurnTimelineCompleted;
		TurnTimelineCompleted.BindUFunction(this, FName("TurnTimelineCompleted"));
		turn_timeline.AddInterpFloat(TurnCurve, TurnTimelineProgress);
		turn_timeline.SetTimelineFinishedFunc(TurnTimelineCompleted);

	}

	if (ForwardCurve) {
		FOnTimelineFloat ForwardTimelineProgress;
		ForwardTimelineProgress.BindUFunction(this, FName("ForwardTimelineProgress"));
		FOnTimelineEvent ForwardTimelineCompleted;
		ForwardTimelineCompleted.BindUFunction(this, FName("ForwardTimelineCompleted"));
		forward_timeline.AddInterpFloat(ForwardCurve, ForwardTimelineProgress);
		forward_timeline.SetTimelineFinishedFunc(ForwardTimelineCompleted);
	}
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	turn_timeline.TickTimeline(DeltaTime);
	forward_timeline.TickTimeline(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAction("MoveBack", IE_Pressed, this, &APlayerPawn::TurnBack);
	PlayerInputComponent->BindAction<FTurnDelegate>("TurnRight", IE_Pressed, this, &APlayerPawn::TurnRight, true);
	PlayerInputComponent->BindAction<FTurnDelegate>("TurnLeft", IE_Pressed, this, &APlayerPawn::TurnRight, false);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerPawn::Interact);
}

void APlayerPawn::TurnRight(bool right) {

	if (!moving) {
		moving = true;

		actual_rotation = target_rotation = PlayerScene->GetComponentRotation();
		int focused = focused_tile.GetValue();

		if (right) {
			target_rotation.Yaw += 90;
			focused++;

			if (focused == D_END) {
				focused = 0;
			}

		}
		else {
			target_rotation.Yaw -= 90;
			focused--;

			if (focused < 0) {
				focused = D_END - 1;
			}
		}

		focused_tile = static_cast<Directions>(focused);

		turn_timeline.PlayFromStart();

		//Set next tile to seen
		ATile* next_tile = *current_tile->neighbours.Find(focused_tile);
		if(next_tile) next_tile->SeeTile();
	}
}

void APlayerPawn::Interact(){
	//if my turn

	//Check if the focused tile has any interactive object

	ATile* next_tile = *current_tile->neighbours.Find(focused_tile);

	if (next_tile->IsValidLowLevel()) {
		if (next_tile->interactive) {

			//Check if we can interact with it (it's facing us)

			 FRotator rot_interactive = next_tile->interactive->GetActorRotation();
			 FRotator rot_mine = PlayerScene->GetComponentRotation();

			 float yaw_interactive = round(rot_interactive.Yaw);
			 float yaw_mine = round(rot_mine.Yaw);

			 if (yaw_interactive == -yaw_mine) {
				 next_tile->interactive->ExecuteInteraction();
			 }
		}
	}
}

void APlayerPawn::TurnTimelineProgress(float alpha)
{
	PlayerScene->SetWorldRotation(FMath::Lerp(FQuat(actual_rotation), FQuat(target_rotation), alpha));
}


void APlayerPawn::TurnTimelineCompleted() {
	moving = false;
}

void APlayerPawn::ForwardTimelineProgress(float alpha){

	this->SetActorLocation(FMath::Lerp(actual_location, target_location, alpha));
}

void APlayerPawn::ForwardTimelineCompleted(){
	actual_location = GetActorLocation();
	moving = false;
}

void APlayerPawn::MoveForward() {

	ATile* next_tile = *current_tile->neighbours.Find(focused_tile);

	if (next_tile && !moving) {
		if (next_tile->can_step_up && !next_tile->reserved) {
			
			next_tile->reserved = true;
			current_tile->reserved = false;
			
			target_location = next_tile->GetActorLocation();
			moving = true;
			current_tile = next_tile;

			forward_timeline.PlayFromStart();

			//Set next tile to seen
			next_tile = *current_tile->neighbours.Find(focused_tile);
			if(next_tile) next_tile->SeeTile();
		}
	}
}

void APlayerPawn::TurnBack() {

	int focused = focused_tile.GetValue();
	int last_focused = focused;
	focused += 2;

	if (focused >= D_END) {
		focused -= D_END;
	}

	focused_tile = static_cast<Directions>(focused);

	ATile* next_tile = *current_tile->neighbours.Find(focused_tile);

	if (next_tile && !moving) {
		if (next_tile->can_step_up && !next_tile->reserved) {

			next_tile->reserved = true;
			current_tile->reserved = false;

			actual_location = GetActorLocation();
			target_location = next_tile->GetActorLocation();
			moving = true;
			current_tile = next_tile;

			forward_timeline.PlayFromStart();

			//Set next tile to seen
			next_tile = *current_tile->neighbours.Find(focused_tile);
			if(next_tile) next_tile->SeeTile();
		}
	}

	focused_tile = static_cast<Directions>(last_focused);

	/*if (!moving) {
		moving = true;

		actual_rotation = target_rotation = GetActorRotation();
		int focused = focused_tile.GetValue();

		target_rotation.Yaw += 180;
		focused += 2;

		if (focused >= D_END) {
			focused -= D_END;
		}

		focused_tile = static_cast<Directions>(focused);
		turn_timeline.PlayFromStart();
	}*/
}

void APlayerPawn::InitializeMinimapRepresentation() {
	if (!minimap_representation && minimap_type) {

		FTransform transform = GetActorTransform();
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = this;

		ATileMap* tm = current_tile->tilemap;

		if (tm) {
			float offset = tm->minimap_Z_offset;
			FVector location = transform.GetLocation();
			location.Z += (offset + 10);
			minimap_representation = GetWorld()->SpawnActor<AMinimapTileRepresentation>(minimap_type, location, transform.GetRotation().Rotator(), spawn_params);
			
			FAttachmentTransformRules rules( EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false );
			//minimap_representation->AttachToComponent(SceneComponent, rules);
			minimap_representation->AttachToActor(this, rules);
		}
	}
}
