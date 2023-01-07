// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObject.generated.h"

UCLASS()
class DCRAWLER_API ABaseObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObject();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Root")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Representation")
		UStaticMeshComponent* object;

	// FUNCTIONS

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
		void AttackAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
