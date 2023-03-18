// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapTileRepresentation.generated.h"

UCLASS()
class GOLDENSACRA_API AMinimapTileRepresentation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinimapTileRepresentation();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* mesh;

	UFUNCTION(BlueprintCallable, Category = "Color")
		void TileHasBeenSeen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
