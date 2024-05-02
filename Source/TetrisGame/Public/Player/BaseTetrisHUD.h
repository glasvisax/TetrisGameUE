#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseTetrisHUD.generated.h"
class UUserWidget;

UCLASS()
class TETRISGAME_API ABaseTetrisHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void TurnOnMenu();

	UFUNCTION(BlueprintCallable)
	void TurnOnDifficultySelection();

	UFUNCTION(BlueprintCallable)
	void TurnGameProgress();

	UFUNCTION(BlueprintCallable)
	void TurnOnGameOver();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DifficultySelectionWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameProgressWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UFUNCTION(BlueprintCallable)
	void SetGameFacade(TScriptInterface<IBaseGameFacade> Facade);

	UFUNCTION(BlueprintCallable)
	TScriptInterface<IBaseGameFacade> GetGameFacade() const { return GameFacade; }

private:

	TScriptInterface<IBaseGameFacade> GameFacade;

	UUserWidget* MenuWidget;

	UUserWidget* DifficultySelectionWidget;

	UUserWidget* GameProgressWidget;

	UUserWidget* GameOverWidget;

};
