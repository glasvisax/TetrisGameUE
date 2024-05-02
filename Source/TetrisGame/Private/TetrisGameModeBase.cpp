#include "TetrisGameModeBase.h"
#include "Block/BaseBlock.h"
#include "BaseTetrisPawn.h"
#include "BlocksShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisPlayerState.h"
#include "TetrisPlayerController.h"
#include "BaseBlockIterator.h"
#include "Player/BaseTetrisHUD.h"

constexpr int32 TetrisFloor = 0;

ATetrisGameModeBase::ATetrisGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATetrisGameModeBase::StartTetrisGame()
{
    ClearAllBlocks();
    FloorPositionZ = BlockEdgeLength * TetrisFloor;
    RoofPositionZ = BlockEdgeLength * TerisRoofIndex;
    BoundingMatrixLength = TetrisMatrixOrder * 100 + 100;

    UE_LOG(LogTemp, Error, TEXT("BoundingMatrixLength : %f"), BoundingMatrixLength);

    TArray<ABaseBlock*> FloorBlocks;
    TArray<ABaseBlock*> RoofBlocks;

    SpawnMatrix(TetrisMatrixOrder, FVector{ 0.0f, 0.0f, FloorPositionZ }, FloorBlocks);

    SpawnMatrix(TetrisMatrixOrder, FVector{ 0.0f, 0.0f, RoofPositionZ }, RoofBlocks);

    for (int i = 0; i < TetrisMatrixOrder * TetrisMatrixOrder; ++i)
    {
        FloorBlocks[i]->SetUpperCheckered();
        RoofBlocks[i]->SetLowerCheckered();
    }

    ResetTetrisPawn();

    const auto TetrisPawn = GetTetrisPawn();
    if (!TetrisPawn) { checkNoEntry(); return; }

    const auto BlocksShapeComponent = TetrisPawn->FindComponentByClass<UBlocksShapeComponent>();
    if (!BlocksShapeComponent) { checkNoEntry(); return; }

    BlocksShapeComponent->OnReachedFloor.AddUObject(this, &ATetrisGameModeBase::OnPawnReachedFloor);
    BlocksShapeComponent->OnReachedRoof.AddUObject(this, &ATetrisGameModeBase::OnPawnReachedRoof);
    BlocksShapeComponent->OnMoveDown.AddUObject(this, &ATetrisGameModeBase::OnMoveDown);
    BlocksShapeComponent->BoundingMatrixLength = BoundingMatrixLength;
    BlocksShapeComponent->FloorPositionZ = FloorPositionZ;
    BlocksShapeComponent->RoofPositionZ = RoofPositionZ;
}

void ATetrisGameModeBase::EndTetrisGame()
{
    GetWorld()->GetTimerManager().ClearTimer(MoveDownTimerHandle);

    const auto Controller = ATetrisPlayerController::GetInstance();
    if (Controller) 
    {
        const auto TetrisPawn  = Cast<ABaseTetrisPawn>(Controller->GetPawn());

        const auto BlocksShapeComponent = TetrisPawn->FindComponentByClass<UBlocksShapeComponent>();
        if (BlocksShapeComponent) 
        { 
            BlocksShapeComponent->ClearShapeActor();
        }
        if (TetrisPawn) 
        {
            TetrisPawn->DisableActionInput();
        }
        const auto TetrisHUD = Cast<ABaseTetrisHUD>(Controller->GetHUD());
        if (TetrisHUD) 
        {
            TetrisHUD->TurnOnGameOver();
        }
    }
}

APlayerController* ATetrisGameModeBase::SpawnPlayerControllerCommon(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation, TSubclassOf<APlayerController> InPlayerControllerClass)
{
    if (ATetrisPlayerController::StaticClass() != InPlayerControllerClass) 
    {
        return Super::SpawnPlayerControllerCommon(InRemoteRole, SpawnLocation, SpawnRotation, InPlayerControllerClass);
    }

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = GetInstigator();
    SpawnInfo.ObjectFlags |= RF_Transient;
    SpawnInfo.bDeferConstruction = true;
    ATetrisPlayerController::SpawnInstance(GetWorld(), SpawnLocation, SpawnRotation, SpawnInfo);
    APlayerController* NewPC = ATetrisPlayerController::GetInstance();
    if (NewPC)
    {
        if (InRemoteRole == ROLE_SimulatedProxy)
        {
            NewPC->SetAsLocalPlayerController();
        }

        UGameplayStatics::FinishSpawningActor(NewPC, FTransform(SpawnRotation, SpawnLocation));
    }
    return NewPC;
}

void ATetrisGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    check(TetrisMatrixOrder % 2 != 0);
}

void ATetrisGameModeBase::MoveDown()
{
    const auto TetrisPawn = GetTetrisPawn();
    if(TetrisPawn)
    {
        TetrisPawn->MoveDown();
    }
}

void ATetrisGameModeBase::SpawnMatrix(int32 MatrixSize, const FVector& Position, TArray<ABaseBlock*>& Blocks)
{
    if (!BlockClass) checkNoEntry();
    
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
            Blocks.Add(NewBlock);

            IndexX++;
        }
        IndexX = -MatrixSize / 2;
        IndexY++;
    }
}

void ATetrisGameModeBase::ResetTetrisPawn()
{
    const auto TetrisPawn = GetTetrisPawn();
    if (!TetrisPawn || CurrentTetrisShapes.Num() == 0) { checkNoEntry(); return; }

    const auto BlocksShapeComponent = TetrisPawn->FindComponentByClass<UBlocksShapeComponent>();
    if (!BlocksShapeComponent) { checkNoEntry(); return; }

    BlocksShapeComponent->ClearShapeActor();
    CheckPlaneFilling();
    TetrisPawn->SetActorLocation(FVector{ 0.0f, 0.0f, BlockEdgeLength * (TerisRoofIndex - 1) });
    
    const auto RandShape = CurrentTetrisShapes.Num() == 1 ? CurrentTetrisShapes[0] : CurrentTetrisShapes[FMath::RandRange(0, CurrentTetrisShapes.Num() - 1)];
    TArray<ABaseBlock*> SpawnedBlocks;

    for (auto i : RandShape)
    {
        const auto NewBlock = GetWorld()->SpawnActor<ABaseBlock>(BlockClass);
        if (!NewBlock){ checkNoEntry(); return; }
        SpawnedBlocks.Add(NewBlock);
        NewBlock->SetActorRotation(FRotator::ZeroRotator);
        NewBlock->SetActorLocation(NewBlock->GetActorLocation() + i);
    }
    BlocksShapeComponent->CreateAndAttachShapeActor(SpawnedBlocks);
    TetrisPawn->EnableActionInput();
    GetWorld()->GetTimerManager().SetTimer(MoveDownTimerHandle, this, &ATetrisGameModeBase::MoveDown, AutoMoveDownInterval, true);
}

void ATetrisGameModeBase::OnPawnReachedFloor()
{;
    GetWorld()->GetTimerManager().ClearTimer(MoveDownTimerHandle);

    const auto TetrisPawn = GetTetrisPawn();
    if (!TetrisPawn) { checkNoEntry(); return; }

    TetrisPawn->DisableActionInput();
    GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ATetrisGameModeBase::ResetTetrisPawn, AutoMoveDownInterval, false);
}

void ATetrisGameModeBase::CheckPlaneFilling()
{
    const auto TetrisPlayerState = GetTetrisPawn()->GetController()->GetPlayerState<ATetrisPlayerState>();
    if(!TetrisPlayerState) { checkNoEntry(); return; }

    TArray<AActor*> AllBlocks;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBlock::StaticClass(), AllBlocks);
    if (AllBlocks.IsEmpty()) return;

    TArray<AActor*> BlocksToDestroy;
    int32 DescNum = 0;
    int32 MaxBlocksInPlane = TetrisMatrixOrder * TetrisMatrixOrder;
    for (int32 i = TetrisFloor + 1; i < TerisRoofIndex - 1; ++i)
    {
        const auto BlocksInside = AllBlocks.FilterByPredicate([&](const AActor* Block) { return Block->GetActorLocation().Z == i * BlockEdgeLength; });
        if (BlocksInside.Num() == MaxBlocksInPlane)
        {
            BlocksToDestroy.Append(BlocksInside);
            
            TetrisPlayerState->AddPoints(1);
            DescNum++;
        }
        
    }
    if (BlocksToDestroy.IsEmpty()) return;
    for (const auto i : BlocksToDestroy)
    {
        i->Destroy();
    }

    while (DescNum) 
    {
        const auto Pred = [&](ABaseBlock* block) ->bool { return block->GetActorLocation().Z != FloorPositionZ && block->GetActorLocation().Z != RoofPositionZ;  };
        for (auto It = ABaseBlock::GetBaseBlockIterator(GetWorld(), Pred); It; ++It)
        {
            auto Block = It.GetBlock();
            Block->SetActorLocation(Block->GetActorLocation() - FVector(0.0f, 0.0f, BlockEdgeLength));
        }
        DescNum--;
    }

}

ABaseTetrisPawn* ATetrisGameModeBase::GetTetrisPawn()
{
    const auto Controller = ATetrisPlayerController::GetInstance();
    if (!Controller) return nullptr;
    
    return Cast<ABaseTetrisPawn>(Controller->GetPawn());
}

void ATetrisGameModeBase::OnPawnReachedRoof()
{
    UE_LOG(LogTemp, Error, TEXT("End Play"));
    EndTetrisGame();
}

void ATetrisGameModeBase::OnMoveDown()
{
    GetWorld()->GetTimerManager().SetTimer(MoveDownTimerHandle, this, &ATetrisGameModeBase::MoveDown, AutoMoveDownInterval, true);
}

void ATetrisGameModeBase::ClearAllBlocks()
{
    const auto Pred = [&](ABaseBlock* block) ->bool { return true; };
    for (auto It = ABaseBlock::GetBaseBlockIterator(GetWorld(), Pred); It; ++It)
    {
        It.GetBlock()->Destroy();
    }
}
