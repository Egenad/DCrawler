// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility.h"
#include "UObject/NoExportTypes.h"
#include "CustomEvent.generated.h"

/**
 * 
 */
UCLASS()
class GOLDENSACRA_API UCustomEvent : public UObject
{
	GENERATED_BODY()

public:

    TEnumAsByte<EventType> getType() {
        return type;
    }

    void initEvent(TEnumAsByte<EventType> new_type) {
        type = new_type;
    }

private:

    TEnumAsByte<EventType> type;
	
};
