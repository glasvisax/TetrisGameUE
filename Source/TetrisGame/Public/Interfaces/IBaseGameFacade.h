#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TetrisTypes.h"
#include "IBaseGameFacade.generated.h"

UINTERFACE(MinimalAPI)
class UBaseGameFacade : public UInterface
{
	GENERATED_BODY()
};

class TETRISGAME_API IBaseGameFacade
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EGameDifficultyLevel GetDifficultyLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDifficultyLevel(EGameDifficultyLevel Level);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopGame();
};
