// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCustomGameInstance::Init(){
	eventManagerCP = NewObject<UEventManagerObject>();
}

void UCustomGameInstance::PlayLevelAudio(USoundBase* sound_to_play, float volume, bool persistent_audio) {
	actual_sound = UGameplayStatics::CreateSound2D(GetWorld(), sound_to_play, volume, 1, 0, nullptr, persistent_audio, true);
	actual_sound->Play(0);
}

void UCustomGameInstance::addEvent(UCustomEvent* event){
	if(eventManagerCP->IsValidLowLevel())
		eventManagerCP->addEvent(event);
}

void UCustomGameInstance::addListener(TEnumAsByte<EventType> typeEvent, TFunction<void()> funcRef){
	if(eventManagerCP->IsValidLowLevel())
		eventManagerCP->addListener(typeEvent, funcRef);
}