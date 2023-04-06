// Fill out your copyright notice in the Description page of Project Settings.


#include "EventManagerComponent.h"

// Sets default values for this component's properties
UEventManagerComponent::UEventManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEventManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEventManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	dispatchEvents();
}

void UEventManagerComponent::dispatchEvents() {
	while (!eventQueue.IsEmpty()) {

		UCustomEvent* event = nullptr;
		
		if (eventQueue.Dequeue(event) && event != nullptr) {
			auto &process = *listeners->Find(event->getType());
			process();
		}
	}
}

void UEventManagerComponent::addEvent(UCustomEvent* newEvent) {
	eventQueue.Enqueue(newEvent);
}

void UEventManagerComponent::addListener(TEnumAsByte<Type> typeEvent, TFunction<void()> funcRef) {
	listeners->Add(typeEvent, funcRef);
}

void UEventManagerComponent::clearEvents() {
	eventQueue.Empty();
}
