// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCustomGameInstance::PlayLevelAudio(USoundBase* sound_to_play, float volume, bool persistent_audio) {
	actual_sound = UGameplayStatics::CreateSound2D(GetWorld(), sound_to_play, volume, 1, 0, nullptr, persistent_audio, true);
	actual_sound->Play(0);
}