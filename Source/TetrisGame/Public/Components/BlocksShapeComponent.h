#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseBlock.h"
#include "BlocksShapeComponent.generated.h"

class ABaseShapeActor;
DECLARE_MULTICAST_DELEGATE(FOnReachedFloorSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TETRISGAME_API UBlocksShapeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBlocksShapeComponent();

protected:

	virtual void BeginPlay() override;

public:	
	FOnReachedFloorSignature OnReachedFloor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float BlockMovementDistance = 200.0f;

	// сторона квадрата ограничивающего плоскость тетриса
	float BoundingMatrixLength;
	float FloorPositionZ = 0.0f;
	float RoofPositionZ = BlockMovementDistance;

	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void MakeRotation();
	
	void CreateAndAttachShapeActor(TArray<ABaseBlock*> BlocksToAttach);

	void ClearShapeActor();

private:

	UPROPERTY()
	ABaseShapeActor* ShapeActor;

	bool IsCanMoveToLocation(const FVector& NewLocation);
	void TryToMove(const FVector& MoveTo);
	void TryToRotate(const FRotator& NewRotation);
	void CheckFloorReaching();
};
