#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TetrisPlayerState.generated.h"

UCLASS()
class TETRISGAME_API ATetrisPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	void AddPoints(int32 PointsToAdd) 
	{
		if (PointsToAdd > 0) 
		{
			Points = Points + PointsToAdd;
		}
	}

	UFUNCTION(BlueprintCallable)
	int32 GetPoints() const { return Points; }

private:
	int32 Points;
	
};
