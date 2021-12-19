// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(SceneComponent);

	character = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character"));
	character->SetupAttachment(SceneComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Assets/Mesh/BasePlane.BasePlane'"));

	if (MeshAsset.Succeeded()) {
		UStaticMesh* Asset = MeshAsset.Object;
		character->SetStaticMesh(Asset);
	}

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

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

}

void APlayerPawn::TurnRight(bool right) {

	if (!moving) {
		moving = true;

		actual_rotation = target_rotation = GetActorRotation();
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

	}

}

void APlayerPawn::TurnTimelineProgress(float alpha)
{
	this->SetActorRotation(FMath::Lerp(FQuat(actual_rotation), FQuat(target_rotation), alpha));	
}


void APlayerPawn::TurnTimelineCompleted() {
	moving = false;
}

void APlayerPawn::ForwardTimelineProgress(float alpha){

	this->SetActorLocation(FMath::Lerp(actual_location, target_location, alpha));
}

void APlayerPawn::ForwardTimelineCompleted(){
	moving = false;
}

void APlayerPawn::MoveForward() {

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
		}
	}
}

void APlayerPawn::TurnBack() {

	if (!moving) {
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
	}
}
