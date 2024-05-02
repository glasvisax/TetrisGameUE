#include "Player/BaseTetrisHUD.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/IBaseGameFacade.h"

void ABaseTetrisHUD::BeginPlay()
{
	Super::BeginPlay();

	MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
	DifficultySelectionWidget = CreateWidget<UUserWidget>(GetWorld(), DifficultySelectionWidgetClass);
	GameProgressWidget = CreateWidget<UUserWidget>(GetWorld(), GameProgressWidgetClass);
	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);

	if (!MenuWidget || !DifficultySelectionWidget || !GameProgressWidget || !GameOverWidget) { checkNoEntry(); return; };
	MenuWidget->AddToViewport();
	DifficultySelectionWidget->AddToViewport();
	GameProgressWidget->AddToViewport();
	GameOverWidget->AddToViewport();
	TurnOnMenu();
}

void ABaseTetrisHUD::TurnOnMenu()
{
	const auto Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;

	MenuWidget->SetVisibility(ESlateVisibility::Visible);
	DifficultySelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	GameProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ABaseTetrisHUD::TurnOnDifficultySelection()
{
	const auto Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;

	MenuWidget->SetVisibility(ESlateVisibility::Hidden);
	DifficultySelectionWidget->SetVisibility(ESlateVisibility::Visible);
	GameProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ABaseTetrisHUD::TurnGameProgress()
{
	const auto Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->bShowMouseCursor = false;

	MenuWidget->SetVisibility(ESlateVisibility::Hidden);
	DifficultySelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	GameProgressWidget->SetVisibility(ESlateVisibility::Visible);
	GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ABaseTetrisHUD::TurnOnGameOver()
{
	const auto Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;

	MenuWidget->SetVisibility(ESlateVisibility::Hidden);
	DifficultySelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	GameProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	GameOverWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABaseTetrisHUD::SetGameFacade(TScriptInterface<IBaseGameFacade> Facade)
{
	GameFacade = Facade;
}
