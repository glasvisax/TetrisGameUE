#include "BaseShapeActor.h"
#include "BaseBlock.h"
#include "BaseBlockIterator.h"
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
	
	auto It = ABaseBlock::GetBaseBlockIterator(GetWorld(), [&](ABaseBlock* Block) { return Block->GetAttachParentActor() == this; });
	const auto RandColor = Colors[FMath::RandRange(1, Colors.Num() - 1)];
	for (; It.Get(); ++It) 
	{
		auto block = It.Get();
		block->SetColor(RandColor);
	}
}