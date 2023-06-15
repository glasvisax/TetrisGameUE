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
    ShapeActor->AttachedBlocks = {};
    ShapeActor->Destroy();
    ShapeActor = nullptr;
}

void UBlocksShapeComponent::MoveDown()
{
    TryToMove(-FVector::UpVector * BlockMovementDistance);
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

void UBlocksShapeComponent::TryToMove(const FVector& MoveTo)
{
    const auto AttachedBlocks = ShapeActor->AttachedBlocks;
    for (const auto block : AttachedBlocks)
    {
        const auto LocationToCheck = block->GetActorLocation() + MoveTo;
        if (!IsCanMoveToLocation(LocationToCheck)) return;

    }
    GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + MoveTo);
    CheckFloorReaching();
}

void UBlocksShapeComponent::CheckFloorReaching()
{
    TArray<AActor*> AllBlocks;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBlock::StaticClass(), AllBlocks);

    const auto AttachedBlocks = ShapeActor->AttachedBlocks;

    for (const auto i : AttachedBlocks)
    {
        for (const auto j : AllBlocks) 
        { 
            if (ShapeActor->AttachedBlocks.Contains(j)) continue;

            const auto iLocation = i->GetActorLocation();
            const auto jLocation = j->GetActorLocation();

            bool a = FMath::IsNearlyEqual(BlockMovementDistance, iLocation.Z - jLocation.Z);
            bool b = (iLocation.X == jLocation.X) && (iLocation.Y == jLocation.Y);
            if (a && b)
            {
                OnReachedFloor.Broadcast();
                return;
            }
        }
    }
}
void UBlocksShapeComponent::TryToRotate(const FRotator& NewRotation)
{
    ShapeActor->AddActorLocalRotation(NewRotation);

    for (const auto block : ShapeActor->AttachedBlocks) 
    {
        if (!IsCanMoveToLocation(block->GetActorLocation())) 
        {
            ShapeActor->AddActorLocalRotation(NewRotation.GetInverse());
            return;
        }
    }
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
