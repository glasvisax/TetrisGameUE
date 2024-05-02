#pragma once
#include "TetrisTypes.generated.h"

UENUM(BlueprintType)
enum class EGameDifficultyLevel : uint8
{
	Easy	UMETA(DisplayName = "Easy"),
	Normal	UMETA(DisplayName = "Normal"),
	Hard	UMETA(DisplayName = "Hard")
};