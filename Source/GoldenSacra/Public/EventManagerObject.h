// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomEvent.h"
#include "Containers/Queue.h"
#include "EventManagerObject.generated.h"


UCLASS()
class GOLDENSACRA_API UEventManagerObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:	

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable, Category = "Event System")
		void addEvent(UCustomEvent* newEvent);

	void addListener(TEnumAsByte<EventType> typeEvent, TFunction<void()> funcRef);

	UFUNCTION(BlueprintCallable, Category = "Event System")
		void clearEvents();

private:

	void dispatchEvents();
	TQueue<UCustomEvent*> eventQueue;
	TMap<TEnumAsByte<EventType>, TFunction<void()>> listeners;
};
