#include "TetrisPlayerController.h"
#include "GameFramework/HUD.h"
#include "Player/BaseTetrisHUD.h"
#include "Player/BaseTetrisFacade.h"

DEFINE_LOG_CATEGORY_STATIC(LogTetrisPlayerController, All, All);

ATetrisPlayerController* ATetrisPlayerController::Instance = nullptr;

ATetrisPlayerController::ATetrisPlayerController()
{

}

void ATetrisPlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	if (ABaseTetrisHUD::StaticClass() == NewHUDClass || NewHUDClass->IsChildOf(ABaseTetrisHUD::StaticClass()))
	{
		if (MyHUD != NULL)
		{
			MyHUD->Destroy();
			MyHUD = NULL;
		}
		
		const auto TetrisFacade = GetWorld()->SpawnActor<ABaseTetrisFacade>();

		const auto TetrisHUD = GetWorld()->SpawnActorDeferred<ABaseTetrisHUD>(NewHUDClass, FTransform());
		if (TetrisHUD) 
		{
			TetrisHUD->SetOwner(this);
			TetrisHUD->SetInstigator(GetInstigator());
			TetrisHUD->SetLifeSpan(0.0f);
			TetrisHUD->SetGameFacade(TetrisFacade);
			TetrisHUD->FinishSpawning(FTransform());
			UE_LOG(LogTetrisPlayerController, Display, TEXT("Tetris HUD was spawned"))
		}
		
		MyHUD = TetrisHUD;
	}
	else Super::ClientSetHUD_Implementation(NewHUDClass);
}

ATetrisPlayerController* ATetrisPlayerController::GetInstance()
{
	return ATetrisPlayerController::Instance;
}

void ATetrisPlayerController::SpawnInstance(UWorld* WorldContextObj, const FVector& SpawnLocation, const FRotator& SpawnRotation, const FActorSpawnParameters& SpawnInfo)
{
	if (ATetrisPlayerController::Instance) return;

	ATetrisPlayerController* NewTPC = WorldContextObj->SpawnActor<ATetrisPlayerController>(ATetrisPlayerController::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
	if (!NewTPC) { checkNoEntry(); return; }
	ATetrisPlayerController::Instance = NewTPC;
}

void ATetrisPlayerController::Shutdown()
{
	Instance->Destroy();
	Instance = nullptr;
}

void ATetrisPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Shutdown();
	Super::EndPlay(EndPlayReason);

}
