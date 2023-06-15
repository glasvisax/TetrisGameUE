#include "TetrisGameModeBase.h"
#include "BaseBlock.h"
#include "Kismet/GameplayStatics.h"
#include "BaseTetrisPawn.h"
#include "Components/BlocksShapeComponent.h"

ATetrisGameModeBase::ATetrisGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATetrisGameModeBase::BeginPlay()
{
    Super::BeginPlay();

	float BoundingMatrixLength = SpawnMatrix(TetrixMatrixOrder, FVector{0.0f, 0.0f, 0.0f});

    SpawnMatrix(TetrixMatrixOrder, FVector{ 0.0f, 0.0f, BlockEdgeLength * TerisRoof });

    const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (!Controller) 
    {
        checkNoEntry();
        return;
    }

    const auto TetrisPawn = Cast<ABaseTetrisPawn>(Controller->GetPawn());
    if (!TetrisPawn) 
    { 
        checkNoEntry();
        return;
    }

    TetrisPawn->SetActorLocation(FVector{ 0.0f, 0.0f, BlockEdgeLength * (TerisRoof - 1) });
    
    const auto BlocksShapeComponent = TetrisPawn->FindComponentByClass<UBlocksShapeComponent>();
    if (!BlocksShapeComponent)
    {
        checkNoEntry();
        return;
    }
    //const auto RandShape = TetrisShapes[(FMath::Rand() % (TetrisShapes.Num()-1))];
    TArray<ABaseBlock*> SpawnedBlocks;
    for (auto i : TetrisShapes[0])
    {
        const auto NewBlock = GetWorld()->SpawnActor<ABaseBlock>(BlockClass);
        if (!NewBlock)
        {
            checkNoEntry();
            return;
        }
        SpawnedBlocks.Add(NewBlock);
        NewBlock->SetActorRotation(FRotator::ZeroRotator);
        NewBlock->SetActorLocation(NewBlock->GetActorLocation() + i);
       
    }
    BlocksShapeComponent->BoundingMatrixLength = BoundingMatrixLength;
    BlocksShapeComponent->CreateAndAttachShapeActor(SpawnedBlocks);
}

float ATetrisGameModeBase::SpawnMatrix(int32 MatrixSize, const FVector& Position)
{
    if (!BlockClass) 
    {
        checkNoEntry(); // не указан класс блока
        return 0.0f;
    }

    float Offset = (MatrixSize - 1) * BlockEdgeLength * 0.5f;

    int32 IndexX = -MatrixSize / 2;
    int32 IndexY = -MatrixSize / 2;

    for (int32 Row = 0; Row < MatrixSize; ++Row)
    {
        for (int32 Column = 0; Column < MatrixSize; ++Column)
        {
            const auto BlockPosition = Position + FVector(IndexX * BlockEdgeLength, IndexY * BlockEdgeLength, 0.0f);

            const auto NewBlock = GetWorld()->SpawnActor<ABaseBlock>(BlockClass);
            if (!NewBlock) checkNoEntry();

            NewBlock->SetActorLocation(BlockPosition);

            IndexX++;
        }

        IndexX = -MatrixSize / 2;
        IndexY++;
    }

    const auto RightBottomBorder = Position + FVector((-MatrixSize / 2) * BlockEdgeLength, (-MatrixSize / 2) * BlockEdgeLength, 0.0f) + FVector{-BlockEdgeLength, -BlockEdgeLength, 0.0f};
    const auto LeftTopBorder = Position + FVector((MatrixSize / 2) * BlockEdgeLength, (MatrixSize / 2) * BlockEdgeLength, 0.0f) + FVector{ BlockEdgeLength, BlockEdgeLength, 0.0f };
    return LeftTopBorder.X;
}