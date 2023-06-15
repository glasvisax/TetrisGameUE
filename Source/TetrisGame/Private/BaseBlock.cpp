#include "BaseBlock.h"
#include "TetrisGameModeBase.h"

ABaseBlock::ABaseBlock()
{
	PrimaryActorTick.bCanEverTick = true;
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockMesh");
	SetRootComponent(BlockMesh);
}

void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


