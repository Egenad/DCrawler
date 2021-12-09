// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/TimelineComponent.h"
#include "TileMap.h"

#include "PlayerPawn.generated.h"

UCLASS()
class DCRAWLER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	//FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "Player")
		void MoveForward();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void TurnBack();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void TurnRight(bool right);

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
