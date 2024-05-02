#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetrisPlayerController.generated.h"

UCLASS()
class TETRISGAME_API ATetrisPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	ATetrisPlayerController();

	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;

public:

	static ATetrisPlayerController* GetInstance();
	static void SpawnInstance(UWorld* WorldContextObj, const FVector& SpawnLocation, const FRotator& SpawnRotation, const FActorSpawnParameters& SpawnInfo);
	static void Shutdown();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	static ATetrisPlayerController* Instance;
};