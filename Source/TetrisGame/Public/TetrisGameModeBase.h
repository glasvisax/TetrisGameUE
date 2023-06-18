// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameModeBase.generated.h"

class ABaseBlock;
class ABaseTetrisPawn;

UCLASS()
class TETRISGAME_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

    ATetrisGameModeBase();

protected:

	virtual void BeginPlay() override;

	float SpawnMatrix(int32 MatrixSize, const FVector& Position);
    
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        Super::PostEditChangeProperty(PropertyChangedEvent);
        if (TetrisMatrixOrder % 2 == 0)
        {
            TetrisMatrixOrder--;
        }
    }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "50", ClampMax = "300"))
	float BlockEdgeLength = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings")
	TSubclassOf<ABaseBlock> BlockClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "3", ClampMax = "21"))
	int32 TetrisMatrixOrder = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "7", ClampMax = "15"))
	int32 TerisRoof = 7;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "0.3", ClampMax = "20.0"))
    float AutoMoveDownInterval = 1.0f;

    UFUNCTION(BlueprintCallable)
    void ResetTetrisPawn();

private:

    FTimerHandle MoveDownTimerHandle;
    FTimerHandle DelayTimerHandle;
    ABaseTetrisPawn* GetTetrisPawn();
    void CheckPlaneFilling();
    

    UFUNCTION()
    void MoveDown();

    float FloorPositionZ;
    float RoofPositionZ;
    float BoundingMatrixLength;

    UFUNCTION()
    void OnPawnReachedFloor();

    UFUNCTION()
    void OnPawnReachedRoof();

    TArray<TArray<FVector>> TetrisShapes =
    {
        
        // I-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{BlockEdgeLength, 0.0f, 0.0f},
          FVector{2 * BlockEdgeLength, 0.0f, 0.0f}, FVector{3 * BlockEdgeLength, 0.0f, 0.0f} },
        // J-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{0.0f, -BlockEdgeLength, 0.0f},
          FVector{0.0f, -2 * BlockEdgeLength, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f} },
        // L-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f},
          FVector{2 * BlockEdgeLength, 0.0f, 0.0f}, FVector{0.0f, -BlockEdgeLength, 0.0f} },
        // O-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f},
          FVector{0.0f, -BlockEdgeLength, 0.0f}, FVector{ BlockEdgeLength, -BlockEdgeLength, 0.0f} },
        // S-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f},
          FVector{0.0f, -BlockEdgeLength, 0.0f}, FVector{-BlockEdgeLength, -BlockEdgeLength, 0.0f} },
        // T-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f},
          FVector{-BlockEdgeLength, 0.0f, 0.0f}, FVector{0.0f, -BlockEdgeLength, 0.0f} },
        // Z-Shape
        { FVector{0.0f, 0.0f, 0.0f}, FVector{-BlockEdgeLength, 0.0f, 0.0f},
          FVector{0.0f, -BlockEdgeLength, 0.0f}, FVector{ BlockEdgeLength, -BlockEdgeLength, 0.0f} },
        // 3-x Line
        { FVector{0.0f, 0.0f, 0.0f}, FVector{-BlockEdgeLength, 0.0f, 0.0f}, FVector{BlockEdgeLength, 0.0f, 0.0f} },
        // 2-x Line
        { FVector{0.0f, 0.0f, 0.0f}, FVector{-BlockEdgeLength, 0.0f, 0.0f} },
        // Point
        { FVector{0.0f, 0.0f, 0.0f} }
    };
};
