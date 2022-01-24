#pragma once

#include "Utility.generated.h"

UENUM()
enum Directions {
	D_UP		UMETA(DisplayName = "Up"),
	D_RIGHT		UMETA(DisplayName = "Right"),
	D_DOWN		UMETA(DisplayName = "Down"),
	D_LEFT		UMETA(DisplayName = "Left"),
	D_END
};

UENUM()
enum TileType {
	TT_Null		UMETA(DisplayName = "Null"),
	TT_Void		UMETA(DisplayName = "Void"),
	TT_Terrain	UMETA(DisplayName = "Terrain"),
	TT_Wall		UMETA(DisplayName = "Wall"),
	TT_Ice		UMETA(DisplayName = "Ice"),
	TT_Gate		UMETA(DisplayName = "Gate")
};

USTRUCT(BlueprintType)
struct FCoord {

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 c_x;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 c_y;
};

USTRUCT(BlueprintType)
struct FDirectionData {

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		FCoord direction;

	UPROPERTY(BlueprintReadOnly)
		FRotator rotation;
};