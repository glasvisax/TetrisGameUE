#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBlock.h"
#include "BaseShapeActor.generated.h"

UCLASS()
class TETRISGAME_API ABaseShapeActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseShapeActor();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneRootComponent;

public:	

	TArray<ABaseBlock*> AttachedBlocks;
	TArray<FLinearColor> Colors = {
		FLinearColor::Red, FLinearColor::Green, FLinearColor::Blue, FLinearColor::White, FLinearColor::Yellow
	};

};
