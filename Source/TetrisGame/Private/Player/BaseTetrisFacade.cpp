#include "Player/BaseTetrisFacade.h"
#include "TetrisGameModeBase.h"

ABaseTetrisFacade::ABaseTetrisFacade()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseTetrisFacade::StartGame_Implementation()
{
	const auto TetrisGameMode = GetTetrisGameMode();
	switch (DifficultyLevel)
	{
	case EGameDifficultyLevel::Easy:
		SetEasyGame();
		break;
	case EGameDifficultyLevel::Normal:
		SetNormalGame();
		break;
	case EGameDifficultyLevel::Hard:
		SetHardGame();
		break;
	default:
		SetEasyGame();
		break;
	}

	TetrisGameMode->StartTetrisGame();
}

void ABaseTetrisFacade::StopGame_Implementation()
{
	const auto TetrisGameMode = GetTetrisGameMode();
	TetrisGameMode->EndTetrisGame();
}

void ABaseTetrisFacade::BeginPlay()
{
	Super::BeginPlay();
	SetEasyGame();

}

void ABaseTetrisFacade::SetEasyGame()
{
	const auto TetrisGameMode = GetTetrisGameMode();

	TetrisGameMode->SetTetrisMatrixOrder(3);
	TetrisGameMode->SetTerisRoofIndex(7);
	TetrisGameMode->SetAutoMoveDownInterval(1.0f);

	TArray<TArray<FVector>> TetrisShapes = { TetrisGameMode->X1_Point, TetrisGameMode->X2_Line };
	TetrisGameMode->SetCurrentTetrisShapes(TetrisShapes);
}

void ABaseTetrisFacade::SetNormalGame()
{
	const auto TetrisGameMode = GetTetrisGameMode();
	TetrisGameMode->SetTetrisMatrixOrder(5);
	TetrisGameMode->SetTerisRoofIndex(7);
	TetrisGameMode->SetAutoMoveDownInterval(0.8f);

	TArray<TArray<FVector>> TetrisShapes = { TetrisGameMode->X1_Point, TetrisGameMode->X3_Line, TetrisGameMode->X2_Line, TetrisGameMode->L_Shape, TetrisGameMode->O_Shape };
	TetrisGameMode->SetCurrentTetrisShapes(TetrisShapes);

}

void ABaseTetrisFacade::SetHardGame()
{
	const auto TetrisGameMode = GetTetrisGameMode();
	TetrisGameMode->SetTetrisMatrixOrder(7);
	TetrisGameMode->SetTerisRoofIndex(7);
	TetrisGameMode->SetAutoMoveDownInterval(0.6f);

	TArray<TArray<FVector>> TetrisShapes = { TetrisGameMode->I_Shape, TetrisGameMode->J_Shape, TetrisGameMode->L_Shape, TetrisGameMode->O_Shape, 
		TetrisGameMode->S_Shape, TetrisGameMode->T_Shape, TetrisGameMode->Z_Shape, TetrisGameMode->X3_Line, TetrisGameMode->X2_Line, TetrisGameMode->X1_Point };
	TetrisGameMode->SetCurrentTetrisShapes(TetrisShapes);
}

ATetrisGameModeBase* ABaseTetrisFacade::GetTetrisGameMode() const
{
	return Cast<ATetrisGameModeBase>(GetWorld()->GetAuthGameMode());
}


