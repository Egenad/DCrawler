// Fill out your copyright notice in the Description page of Project Settings.


#include "EventManagerObject.h"

void UEventManagerObject::Tick(float DeltaTime){
	dispatchEvents();
}

bool UEventManagerObject::IsTickable() const{
	return true;
}

bool UEventManagerObject::IsTickableInEditor() const{
	return false;
}

bool UEventManagerObject::IsTickableWhenPaused() const{
	return false;
}

TStatId UEventManagerObject::GetStatId() const{
	return TStatId();
}

void UEventManagerObject::dispatchEvents() {
	while (!eventQueue.IsEmpty() && !listeners.IsEmpty()) {

		UCustomEvent* event = nullptr;
		
		if (eventQueue.Dequeue(event) && event != nullptr) {
			auto &process = *listeners.Find(event->getType());
			process();
		}
	}
}

void UEventManagerObject::addEvent(UCustomEvent* newEvent) {
	eventQueue.Enqueue(newEvent);
}

void UEventManagerObject::addListener(TEnumAsByte<EventType> typeEvent, TFunction<void()> funcRef) {
	listeners.Add(typeEvent, funcRef);
}

void UEventManagerObject::clearEvents() {
	eventQueue.Empty();
}
