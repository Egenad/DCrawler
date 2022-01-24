// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "TileMap.h"
#include "BaseAbility.h"

#include "PlayerPawn.generated.h"

UCLASS()
class DCRAWLER_API APlayerPawn : public APawn
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

	UFUNCTION(Category = "TurnTimeline")
		void TurnTimelineProgress(float alpha);

	UFUNCTION(Category = "TurnTimeline")
		void TurnTimelineCompleted();

	UFUNCTION(Category = "ForwardTimeline")
		void ForwardTimelineProgress(float alpha);

	UFUNCTION(Category = "ForwardTimeline")
		void ForwardTimelineCompleted();

	//PROPERTIES

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool moving;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FRotator actual_rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FRotator target_rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FVector actual_location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
		FVector target_location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		ATile* current_tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TEnumAsByte<Directions> focused_tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		UCurveFloat* TurnCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		UCurveFloat* ForwardCurve;

	// Comps

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Translation")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UStaticMeshComponent* character;

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
		TArray<TSubclassOf<ABaseAbility>> abilities;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimeline turn_timeline;
	FTimeline forward_timeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
