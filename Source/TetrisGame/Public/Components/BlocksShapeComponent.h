#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseBlock.h"
#include "BlocksShapeComponent.generated.h"
class ABaseShapeActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TETRISGAME_API UBlocksShapeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBlocksShapeComponent();

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float BlockMovementDistance = 200.0f;

	// сторона квадрата ограничивающего плоскость тетриса
	float BoundingMatrixLength;

	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void MakeRotation();
	
	void CreateAndAttachShapeActor(TArray<ABaseBlock*> BlocksToAttach);

private:

	UPROPERTY()
	ABaseShapeActor* ShapeActor;

	bool IsCanMoveToLocation(const FVector& NewLocation);
	void TryToMove(const FVector& MoveTo);
	void TryToRotate(const FRotator& NewRotation);
};
