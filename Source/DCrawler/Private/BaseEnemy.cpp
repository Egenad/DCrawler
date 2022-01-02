// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

void ABaseEnemy::setOrientationToPlayerCamera()
{

	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	APawn* player = controller->GetPawn();

	FVector target_location = player->GetActorLocation();
	FVector my_location = this->GetActorLocation();

	FRotator new_rotation = UKismetMathLibrary::FindLookAtRotation(my_location, target_location);

	FRotator actual_rotation = this->GetActorRotation();
	this->SetActorRotation(FRotator(0, new_rotation.Yaw, 0));

}

void ABaseEnemy::initializeTile()
{
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	setOrientationToPlayerCamera();

}



