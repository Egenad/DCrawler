// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Components/AudioComponent.h"
#include "EventManagerObject.h"

#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOLDENSACRA_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	//Functions

	void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void PlayLevelAudio(USoundBase* sound_to_play, float volume, bool persistent_audio);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void addEvent(UCustomEvent* event);

	void addListener(TEnumAsByte<EventType> typeEvent, TFunction<void()> funcRef);

	//Properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		UAudioComponent* actual_sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		float MAX_LEGAL_LUCK = 100;

private:

	UEventManagerObject* eventManagerCP;
	
};
