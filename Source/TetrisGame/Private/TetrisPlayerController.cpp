#include "TetrisPlayerController.h"

ATetrisPlayerController::ATetrisPlayerController()
{

}

ATetrisPlayerController* ATetrisPlayerController::Instance = nullptr;

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
}
