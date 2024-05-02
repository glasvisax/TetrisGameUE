#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IBaseGameFacade.h"
#include "TetrisTypes.h"
#include "BaseTetrisFacade.generated.h"

class ATetrisGameModeBase;

UCLASS()
class TETRISGAME_API ABaseTetrisFacade : public AActor, public IBaseGameFacade
{
	GENERATED_BODY()
	
public:	

	ABaseTetrisFacade();

	UFUNCTION(BlueprintCallable)
	virtual void StartGame_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual void StopGame_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetDifficultyLevel_Implementation(EGameDifficultyLevel Level) override { DifficultyLevel = Level; }

	UFUNCTION(BlueprintCallable)
	virtual EGameDifficultyLevel GetDifficultyLevel_Implementation() const override { return DifficultyLevel; }

protected:

	virtual void BeginPlay() override;

private:

	EGameDifficultyLevel DifficultyLevel = EGameDifficultyLevel::Easy;

	void SetEasyGame();
	void SetNormalGame();
	void SetHardGame();

	ATetrisGameModeBase* GetTetrisGameMode() const;
};
