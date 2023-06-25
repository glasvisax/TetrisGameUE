#include "TetrisGameModeBase.h"
#include "BaseBlock.h"
#include "BaseTetrisPawn.h"
#include "Components/BlocksShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisPlayerState.h"
#include "TetrisPlayerController.h"
#include "BaseBlockIterator.h"

constexpr int32 TetrisFloor = 0;

ATetrisGameModeBase::ATetrisGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

APlayerController* ATetrisGameModeBase::SpawnPlayerControllerCommon(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation, TSubclassOf<APlayerController> InPlayerControllerClass)
{
    if (ATetrisPlayerController::StaticClass() != InPlayerControllerClass) 
    {
        return Super::SpawnPlayerControllerCommon(InRemoteRole, SpawnLocation, SpawnRotation, InPlayerControllerClass);
    }

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = GetInstigator();
    SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
    SpawnInfo.bDeferConstruction = true;
    ATetrisPlayerController::SpawnInstance(GetWorld(), SpawnLocation, SpawnRotation, SpawnInfo);
    APlayerController* NewPC = ATetrisPlayerController::GetInstance();

    if (NewPC)
    {
        if (InRemoteRole == ROLE_SimulatedProxy)
        {
            // This is a local player because it has no authority/autonomous remote role
            NewPC->SetAsLocalPlayerController();
        }

        UGameplayStatics::FinishSpawningActor(NewPC, FTransform(SpawnRotation, SpawnLocation));
    }
    UE_LOG(LogTemp, Error, TEXT("spawn tetris controller"));
    return NewPC;
}

void ATetrisGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    FloorPositionZ = BlockEdgeLength * TetrisFloor;
    RoofPositionZ = BlockEdgeLength * TerisRoof;

	BoundingMatrixLength = SpawnMatrix(TetrisMatrixOrder, FVector{0.0f, 0.0f, FloorPositionZ });

    SpawnMatrix(TetrisMatrixOrder, FVector{ 0.0f, 0.0f, RoofPositionZ });

    ResetTetrisPawn();

    const auto TetrisPawn = GetTetrisPawn();
    if (!TetrisPawn) { checkNoEntry(); return; }

    const auto BlocksShapeComponent = GetTetrisPawn()->FindComponentByClass<UBlocksShapeComponent>();
    if (!BlocksShapeComponent) { checkNoEntry(); return; }
    BlocksShapeComponent->OnReachedFloor.AddUObject(this, &ATetrisGameModeBase::OnPawnReachedFloor);
    BlocksShapeComponent->OnReachedRoof.AddUObject(this, &ATetrisGameModeBase::OnPawnReachedRoof);
    BlocksShapeComponent->OnMoveDown.AddUObject(this, &ATetrisGameModeBase::OnMoveDown);
    BlocksShapeComponent->BoundingMatrixLength = BoundingMatrixLength;
    BlocksShapeComponent->FloorPositionZ = FloorPositionZ;
    BlocksShapeComponent->RoofPositionZ = RoofPositionZ;
}

void ATetrisGameModeBase::MoveDown()
{
    const auto TetrisPawn = GetTetrisPawn();
    if(TetrisPawn)
    {
        TetrisPawn->MoveDown();
    }
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
    return LeftTopBorder.X - 100.0f;
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
    CheckPlaneFilling();
    TetrisPawn->SetActorLocation(FVector{ 0.0f, 0.0f, BlockEdgeLength * (TerisRoof - 1) });
    
    const auto RandShape = TetrisShapes[FMath::RandRange(1, TetrisShapes.Num() - 1)];
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
    const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (!Controller) { checkNoEntry(); return; }

    const auto TetrisPawn = Cast<ABaseTetrisPawn>(Controller->GetPawn());
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

    int32 MaxBlocksInPlane = TetrisMatrixOrder * TetrisMatrixOrder;
    for (int32 i = TetrisFloor + 1; i < TerisRoof - 1; ++i)
    {
        const auto BlocksInside = AllBlocks.FilterByPredicate([&](const AActor* Block) { return Block->GetActorLocation().Z == i * BlockEdgeLength; });
        if (BlocksInside.Num() == MaxBlocksInPlane)
        {
            BlocksToDestroy.Append(BlocksInside);
            
            TetrisPlayerState->AddPoints(1);
        }
        
    }
    if (BlocksToDestroy.IsEmpty()) return;
    for (const auto i : BlocksToDestroy)
    {
        i->Destroy();
    }
}

ABaseTetrisPawn* ATetrisGameModeBase::GetTetrisPawn()
{
    const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (!Controller) return nullptr;

    return Cast<ABaseTetrisPawn>(Controller->GetPawn());
}

void ATetrisGameModeBase::OnPawnReachedRoof()
{
    UE_LOG(LogTemp, Error, TEXT("End Play"));
}

void ATetrisGameModeBase::OnMoveDown()
{
    // Обновите существующий таймер с новым интервалом
    GetWorld()->GetTimerManager().SetTimer(MoveDownTimerHandle, this, &ATetrisGameModeBase::MoveDown, AutoMoveDownInterval, true);
}
