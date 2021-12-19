// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Containers/Array.h"

#include "Tile.generated.h"

UENUM()
enum Directions {
	D_UP		UMETA(DisplayName = "Up"),
	D_RIGHT		UMETA(DisplayName = "Right"),
	D_DOWN		UMETA(DisplayName = "Down"),
	D_LEFT		UMETA(DisplayName = "Left"),
	D_END
};

UENUM()
enum TileType {
	TT_Null		UMETA(DisplayName = "Null"),
	TT_Void		UMETA(DisplayName = "Void"),
	TT_Terrain	UMETA(DisplayName = "Terrain"),
	TT_Wall		UMETA(DisplayName = "Wall"),
	TT_Ice		UMETA(DisplayName = "Ice")
};

USTRUCT(BlueprintType)
struct FCoord {

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 c_x;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 c_y;
};

UCLASS()
class DCRAWLER_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
