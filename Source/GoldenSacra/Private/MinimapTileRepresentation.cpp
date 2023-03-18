// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapTileRepresentation.h"

// Sets default values
AMinimapTileRepresentation::AMinimapTileRepresentation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(SceneComponent);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(SceneComponent);

}

// Called when the game starts or when spawned
void AMinimapTileRepresentation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinimapTileRepresentation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinimapTileRepresentation::TileHasBeenSeen() {
	mesh->SetVisibility(true);
}