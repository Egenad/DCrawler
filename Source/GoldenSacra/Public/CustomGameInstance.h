// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Components/AudioComponent.h"

#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOLDENSACRA_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void PlayLevelAudio(USoundBase* sound_to_play, float volume, bool persistent_audio);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		UAudioComponent* actual_sound;
	
};
