#include "BaseShapeActor.h"
#include "BaseBlock.h"
#include "Kismet/GameplayStatics.h"

ABaseShapeActor::ABaseShapeActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRootComponent);
}

void ABaseShapeActor::BeginPlay()
{
	Super::BeginPlay();

	const auto Pred = [&](ABaseBlock* Block) -> bool { return Block->GetAttachParentActor() == this; };
	const auto RandColor = Colors[FMath::RandRange(1, Colors.Num() - 1)];

	for (auto It = ABaseBlock::GetBaseBlockIterator(GetWorld(), Pred); It; ++It)
	{
		auto block = It.GetBlock();
		block->SetStaticColor(RandColor);
	}
}