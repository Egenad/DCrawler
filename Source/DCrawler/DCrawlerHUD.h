// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DCrawlerHUD.generated.h"

UCLASS()
class ADCrawlerHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADCrawlerHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

