// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Containers/Array.h"
#include "Utility.h"
#include "Interactive.h"

#include "Tile.generated.h"

class ATileMap;

UCLASS()
class DCRAWLER_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Functions

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Type")
		void ChangeTileType();

	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
		TEnumAsByte<TileType> type;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		FCoord coordinates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Tile")
		TMap<TEnumAsByte<Directions>, ATile*> neighbours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		bool can_step_up = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		bool can_start = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		bool reserved = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
		ATileMap* tilemap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactive")
		AInteractive* interactive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
