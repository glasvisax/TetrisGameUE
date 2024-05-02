#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseBlock.h"
#include "BlocksShapeComponent.generated.h"

class ABaseShapeActor;
DECLARE_MULTICAST_DELEGATE(FBaseDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TETRISGAME_API UBlocksShapeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBlocksShapeComponent();

protected:

	virtual void BeginPlay() override;

public:	
	FBaseDelegate OnReachedFloor;
	FBaseDelegate OnReachedRoof;
	FBaseDelegate OnMoveDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float BlockMovementDistance = 200.0f;

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
	bool TryToMove(const FVector& MoveTo);
	bool TryToRotate(const FRotator& NewRotation);
	void CheckReaching();
	FTimerHandle SmoothMoveHandle;
};
