// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomEvent.h"
#include "Containers/Queue.h"
#include "Components/ActorComponent.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "EventManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLDENSACRA_API UEventManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEventManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Event System")
		void addEvent(UCustomEvent* newEvent);

	void addListener(TEnumAsByte<Type> typeEvent, TFunction<void()> funcRef);

	UFUNCTION(BlueprintCallable, Category = "Event System")
		void clearEvents();

private:

	void dispatchEvents();
	TQueue<UCustomEvent*> eventQueue;
	TMap<TEnumAsByte<Type>, TFunction<void()>> *listeners;
};
