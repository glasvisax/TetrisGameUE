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
    ShapeActor = GetWorld()->SpawnActor<ABaseShapeActor>(ABaseShapeActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
    for (const auto Actor : BlocksToAttach)
    {
        if (Actor)
        {
            Actor->AttachToComponent(ShapeActor->GetRootComponent(), AttachmentRules);
        }
    }
    ShapeActor->AttachToComponent(GetOwner()->GetRootComponent(), AttachmentRules);
    ShapeActor->SetActorRotation(FRotator::ZeroRotator);
    ShapeActor->AttachedBlocks = BlocksToAttach;
    ShapeActor->SetOwner(GetOwner());
}

void UBlocksShapeComponent::MoveDown()
{
    GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() - FVector::UpVector * BlockMovementDistance);
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
        if (!IsCanMoveToLocation(LocationToCheck)) 
        {
            //UE_LOG(LogTemp, Error, TEXT("%s, %s, false"), *block->GetName(), *LocationToCheck.ToString());
            return;
        }
        else 
        {
            //UE_LOG(LogTemp, Error, TEXT("%s, %s, true"), *block->GetName(), *LocationToCheck.ToString());
        }

    }
    GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + MoveTo);

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