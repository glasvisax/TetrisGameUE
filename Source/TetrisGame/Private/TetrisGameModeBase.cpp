#include "TetrisGameModeBase.h"
#include "BaseBlock.h"
#include "BaseTetrisPawn.h"
#include "Components/BlocksShapeComponent.h"
#include "Kismet/GameplayStatics.h"

ATetrisGameModeBase::ATetrisGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATetrisGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    FloorPositionZ = 0.0f;
    RoofPositionZ = BlockEdgeLength * TerisRoof;

	float BoundingMatrixLength = SpawnMatrix(TetrixMatrixOrder, FVector{0.0f, 0.0f, FloorPositionZ });

    SpawnMatrix(TetrixMatrixOrder, FVector{ 0.0f, 0.0f, RoofPositionZ });

    ResetTetrisPawn();

    const auto TetrisPawn = GetTetrisPawn();
    if (!TetrisPawn) { checkNoEntry(); return; }

    const auto BlocksShapeComponent = GetTetrisPawn()->FindComponentByClass<UBlocksShapeComponent>();
    if (!BlocksShapeComponent) { checkNoEntry(); return; }
    BlocksShapeComponent->BoundingMatrixLength = BoundingMatrixLength;
    BlocksShapeComponent->FloorPositionZ = FloorPositionZ;
    BlocksShapeComponent->RoofPositionZ = RoofPositionZ;
}

float ATetrisGameModeBase::SpawnMatrix(int32 MatrixSize, const FVector& Position)
{
    if (!BlockClass) { checkNoEntry(); return 0.0f; }

    float Offset = (MatrixSize - 1) * BlockEdgeLength * 0.5f;

    int32 IndexX = -MatrixSize / 2;
    int32 IndexY = -MatrixSize / 2;

    for (int32 Row = 0; Row < MatrixSize; ++Row)
    {
        for (int32 Column = 0; Column < MatrixSize; ++Column)
        {
            const auto BlockPosition = Position + FVector(IndexX * BlockEdgeLength, IndexY * BlockEdgeLength, 0.0f);

            const auto NewBlock = GetWorld()->SpawnActor<ABaseBlock>(BlockClass);
            if (!NewBlock) { checkNoEntry(); continue; }

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

void ATetrisGameModeBase::ResetTetrisPawn()
{
    const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (!Controller) { checkNoEntry(); return; }

    const auto TetrisPawn = GetTetrisPawn();
    if (!TetrisPawn) { checkNoEntry(); return; }

    const auto BlocksShapeComponent = TetrisPawn->FindComponentByClass<UBlocksShapeComponent>();
    if (!BlocksShapeComponent) { checkNoEntry(); return; }

    BlocksShapeComponent->ClearShapeActor();

    TetrisPawn->SetActorLocation(FVector{ 0.0f, 0.0f, BlockEdgeLength * (TerisRoof - 1) });
    
    const auto RandShape = TetrisShapes[(FMath::Rand() % (TetrisShapes.Num() - 1))];
    TArray<ABaseBlock*> SpawnedBlocks;
    for (auto i : RandShape)
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
    BlocksShapeComponent->CreateAndAttachShapeActor(SpawnedBlocks);
    BlocksShapeComponent->OnReachedFloor.AddUObject(this, &ATetrisGameModeBase::OnPawnReachedFloor);
    TetrisPawn->EnableInput(Controller);
}

ABaseTetrisPawn* ATetrisGameModeBase::GetTetrisPawn()
{
    const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (!Controller) return nullptr;

    return Cast<ABaseTetrisPawn>(Controller->GetPawn());
}

void ATetrisGameModeBase::OnPawnReachedFloor()
{
    // какая то логика
    const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (!Controller) { checkNoEntry(); return; }

    const auto TetrisPawn = Cast<ABaseTetrisPawn>(Controller->GetPawn());
    if (!TetrisPawn) { checkNoEntry(); return; }

    TetrisPawn->DisableInput(Controller);
    FTimerHandle DelayTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ATetrisGameModeBase::ResetTetrisPawn, 2.0f);
}
