#include "Components/BlocksShapeComponent.h"
#include "BaseShapeActor.h"
#include "Kismet/GameplayStatics.h"

UBlocksShapeComponent::UBlocksShapeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBlocksShapeComponent::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner());
}
void UBlocksShapeComponent::CreateAndAttachShapeActor(TArray<ABaseBlock*> BlocksToAttach)
{
    if (ShapeActor) return;
    ShapeActor = GetWorld()->SpawnActor<ABaseShapeActor>(ABaseShapeActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    ShapeActor = GetWorld()->SpawnActorDeferred<ABaseShapeActor>(ABaseShapeActor::StaticClass(), FTransform());
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
    for (const auto block : BlocksToAttach)
    {
        if (block)
        {
            const auto CurrentLocation = block->GetActorLocation();
            block->AttachToActor(ShapeActor, AttachmentRules);
            block->SetActorLocation(CurrentLocation);
        }
    }
    ShapeActor->FinishSpawning(FTransform());
    ShapeActor->AttachToComponent(GetOwner()->GetRootComponent(), AttachmentRules);
    ShapeActor->SetActorRotation(FRotator::ZeroRotator);
    ShapeActor->AttachedBlocks = BlocksToAttach;
    ShapeActor->SetOwner(GetOwner());
}

void UBlocksShapeComponent::ClearShapeActor()
{
    if (!ShapeActor) return;

    for (const auto block : ShapeActor->AttachedBlocks)
    {
        if (block) 
        {
            block->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        }
    }
    ShapeActor->AttachedBlocks.Empty();
    ShapeActor->Destroy();
    ShapeActor = nullptr;
}

void UBlocksShapeComponent::MoveDown()
{
    if (TryToMove(-FVector::UpVector * BlockMovementDistance)) 
    {
        OnMoveDown.Broadcast();
    }
}

void UBlocksShapeComponent::MoveLeft()
{
    TryToMove(-(FVector::RightVector * BlockMovementDistance));
}

void UBlocksShapeComponent::MoveRight()
{
    TryToMove((FVector::RightVector * BlockMovementDistance));
}
void UBlocksShapeComponent::MoveForward() 
{
    TryToMove(FVector::ForwardVector * BlockMovementDistance);
}

void UBlocksShapeComponent::MoveBackward()
{
    TryToMove( -(FVector::ForwardVector * BlockMovementDistance));
}

void UBlocksShapeComponent::MakeRotation()
{
    const auto Rotation = FRotator{ 0.0f, 90.0f, 0.0f };
    TryToRotate(Rotation);
}

bool UBlocksShapeComponent::TryToMove(const FVector& MoveTo)
{
    const auto AttachedBlocks = ShapeActor->AttachedBlocks;
    for (const auto block : AttachedBlocks)
    {
        const auto LocationToCheck = block->GetActorLocation() + MoveTo;
        if (!IsCanMoveToLocation(LocationToCheck)) { CheckReaching(); return false; }
    }
    GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + MoveTo);
    return true;
}

void UBlocksShapeComponent::CheckReaching()
{
    TArray<AActor*> AllBlocks;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBlock::StaticClass(), AllBlocks);

    for (const auto attached : ShapeActor->AttachedBlocks)
    {
        for (const auto non_attached : AllBlocks)
        { 
            if (ShapeActor->AttachedBlocks.Contains(non_attached)) continue;

            const auto AttachedLocation = attached->GetActorLocation();
            const auto NonAttachedLocation = non_attached->GetActorLocation();

            bool IsFloorReaching = FMath::IsNearlyEqual(BlockMovementDistance, AttachedLocation.Z - NonAttachedLocation.Z) && 
                FMath::IsNearlyEqual(NonAttachedLocation.X, AttachedLocation.X) && 
                FMath::IsNearlyEqual(NonAttachedLocation.Y, AttachedLocation.Y);

            bool IsRoofReaching = FMath::IsNearlyEqual(BlockMovementDistance, RoofPositionZ - AttachedLocation.Z);
            if (IsRoofReaching && IsFloorReaching)
            {
                OnReachedRoof.Broadcast();
                return;
            }

            if (IsFloorReaching)
            {
                OnReachedFloor.Broadcast();
                return;
            }
        }
    }
}
bool UBlocksShapeComponent::TryToRotate(const FRotator& NewRotation)
{
    ShapeActor->AddActorLocalRotation(NewRotation);

    for (const auto block : ShapeActor->AttachedBlocks) 
    {
        if (!IsCanMoveToLocation(block->GetActorLocation())) 
        {
            ShapeActor->AddActorLocalRotation(NewRotation.GetInverse());
            return false;
        }
    }
    return true;
}

bool UBlocksShapeComponent::IsCanMoveToLocation(const FVector& NewLocation)
{
    TArray<AActor*> AllBlocks;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBlock::StaticClass(), AllBlocks);

    for (const auto block : AllBlocks) 
    {
        if (ShapeActor->AttachedBlocks.Contains(block)) continue;

        if (NewLocation.Equals(block->GetActorLocation(), 0.2f)) return false;
    }
    return (NewLocation.X > -BoundingMatrixLength) && (NewLocation.X < BoundingMatrixLength) && 
           (NewLocation.Y > -BoundingMatrixLength) && (NewLocation.Y < BoundingMatrixLength);

}