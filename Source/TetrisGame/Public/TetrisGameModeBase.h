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

    void StartTetrisGame();

    void EndTetrisGame();

protected:

    virtual APlayerController* SpawnPlayerControllerCommon(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation, TSubclassOf<APlayerController> InPlayerControllerClass) override;

	virtual void BeginPlay() override;

	void SpawnMatrix(int32 MatrixSize, const FVector& Position, TArray<ABaseBlock*>& Blocks);
    
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "3", ClampMax = "31"))
	int32 TetrisMatrixOrder = 3;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "7", ClampMax = "15"))
	int32 TerisRoofIndex = 7;    
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Settings", meta = (ClampMin = "0.5", ClampMax = "5.0"))
    float AutoMoveDownInterval = 1.0f;

public:

    const TArray<FVector> I_Shape = { FVector{0.0f, 0.0f, 0.0f}, FVector{BlockEdgeLength, 0.0f, 0.0f},
          FVector{2 * BlockEdgeLength, 0.0f, 0.0f}, FVector{3 * BlockEdgeLength, 0.0f, 0.0f} };

    const TArray<FVector> J_Shape = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ 0.0f, -BlockEdgeLength, 0.0f },
          FVector{ 0.0f, -2 * BlockEdgeLength, 0.0f }, FVector{ BlockEdgeLength, 0.0f, 0.0f } };

    const TArray<FVector> L_Shape = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ BlockEdgeLength, 0.0f, 0.0f },
          FVector{ 2 * BlockEdgeLength, 0.0f, 0.0f }, FVector{ 0.0f, -BlockEdgeLength, 0.0f } };

    const TArray<FVector> O_Shape = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ BlockEdgeLength, 0.0f, 0.0f },
          FVector{ 0.0f, -BlockEdgeLength, 0.0f }, FVector{ BlockEdgeLength, -BlockEdgeLength, 0.0f } };

    const TArray<FVector> S_Shape = { FVector{0.0f, 0.0f, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f},
          FVector{0.0f, -BlockEdgeLength, 0.0f}, FVector{-BlockEdgeLength, -BlockEdgeLength, 0.0f} };

    const TArray<FVector> T_Shape = { FVector{0.0f, 0.0f, 0.0f}, FVector{ BlockEdgeLength, 0.0f, 0.0f},
          FVector{-BlockEdgeLength, 0.0f, 0.0f}, FVector{0.0f, -BlockEdgeLength, 0.0f} };

    const TArray<FVector> Z_Shape = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ -BlockEdgeLength, 0.0f, 0.0f },
          FVector{ 0.0f, -BlockEdgeLength, 0.0f }, FVector{ BlockEdgeLength, -BlockEdgeLength, 0.0f } };

    const TArray<FVector> X3_Line = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ -BlockEdgeLength, 0.0f, 0.0f },
          FVector{ BlockEdgeLength, 0.0f, 0.0f } };

    const TArray<FVector> X2_Line = { FVector{ 0.0f, 0.0f, 0.0f }, FVector{ -BlockEdgeLength, 0.0f, 0.0f } };

    const TArray<FVector> X1_Point = { FVector{ 0.0f, 0.0f, 0.0f } };

    void SetTetrisMatrixOrder(int32 Order) { TetrisMatrixOrder = FMath::Clamp(Order % 2 == 0 ? Order - 1 : Order, 3, 31); }

    int32 GetTetrisMatrixOrder() const { return TetrisMatrixOrder; }

    void SetTerisRoofIndex(int32 Index){ TerisRoofIndex = FMath::Clamp(Index, 7, 15); }

    int32 GetTerisRoofIndex() const { return TerisRoofIndex; }

    void SetAutoMoveDownInterval(int32 Index) { AutoMoveDownInterval = FMath::Clamp(AutoMoveDownInterval, 0.5f, 5.0f); }

    int32 GetAutoMoveDownInterval() const { return AutoMoveDownInterval; }

    void SetCurrentTetrisShapes(TArray<TArray<FVector>> Shapes){ CurrentTetrisShapes = Shapes; }

    TArray<TArray<FVector>> GetCurrentTetrisShapes() { return CurrentTetrisShapes; }

private:

    UFUNCTION(BlueprintCallable)
    void ResetTetrisPawn();
    
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

    UFUNCTION()
    void OnMoveDown();

    TArray<TArray<FVector>> CurrentTetrisShapes = { I_Shape, J_Shape, L_Shape, O_Shape, S_Shape, T_Shape, Z_Shape, X3_Line, X2_Line, X1_Point };

    void ClearAllBlocks();
};


