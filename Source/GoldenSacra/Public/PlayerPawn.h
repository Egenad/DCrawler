// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "TileMap.h"
#include "BaseAbility.h"
#include "MinimapTileRepresentation.h"
#include "Components/ChildActorComponent.h"

#include "PlayerPawn.generated.h"

const int MAX_OBJECT_MOVEMENTS = 6;

UCLASS()
class GOLDENSACRA_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	//FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "Action")
		void MoveForward();

	UFUNCTION(BlueprintCallable, Category = "Action")
		void TurnBack();

	UFUNCTION(BlueprintCallable, Category = "Action")
		void TurnRight(bool right);

	UFUNCTION(BlueprintCallable, Category = "Action")
		void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
		void MovePlayer();

	UFUNCTION(BlueprintCallable, Category = "Action")
		void EndTurn();

	UFUNCTION(Category = "TurnTimeline")
		void TurnTimelineProgress(float alpha);

	UFUNCTION(Category = "TurnTimeline")
		void TurnTimelineCompleted();

	UFUNCTION(Category = "Minimap")
		void InitializeMinimapRepresentation();

	UFUNCTION(Category = "SFX")
		void PlayFootstepAudioByTag(FName tag);

	UFUNCTION(Category = "Enemy")
		void SetEnemyHudVisibilityInTile(ATile* next_tile, bool visibility);

	//PROPERTIES

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool moving;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FRotator actual_rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FRotator target_rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		FVector actual_location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FVector target_location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		float actualObjectZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		float targetObjectZ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		ATile* current_tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TEnumAsByte<Directions> focused_tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		UCurveFloat* TurnCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		TMap<FName, USoundBase*> sound_map;

	// Comps

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Root")
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* PlayerScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies")
		USceneComponent* EnemyTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UStaticMeshComponent* character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		UChildActorComponent* right_hand_object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		UChildActorComponent* left_hand_object;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MinimapRepresentation")
		AMinimapTileRepresentation* minimap_representation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinimapRepresentation")
		TSubclassOf<AMinimapTileRepresentation> minimap_type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UCameraComponent* camera;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
		UInputComponent* player_input;

	DECLARE_DELEGATE_OneParam(FTurnDelegate, bool);

	// Gameplay Related

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		int level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float max_health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float base_damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		bool myTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		TArray<TSubclassOf<ABaseAbility>> abilities;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimeline turn_timeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
