// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomEvent.generated.h"

UENUM(BlueprintType)
enum Type {
    TT_PROCESS_TURN
};

/**
 * 
 */
UCLASS()
class GOLDENSACRA_API UCustomEvent : public UObject
{
	GENERATED_BODY()

public:

    TEnumAsByte<Type> getType() {
        return type;
    }

    void initEvent(TEnumAsByte<Type> new_type) {
        type = new_type;
    }

private:

    TEnumAsByte<Type> type;
	
};
