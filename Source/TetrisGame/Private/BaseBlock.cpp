#include "BaseBlock.h"
#include "TetrisGameModeBase.h"
#include "BaseBlockIterator.h"
#include "Kismet/GameplayStatics.h"

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

BaseBlockIterator ABaseBlock::GetBaseBlockIterator(UWorld* WorldContextObj, std::function<bool(ABaseBlock*)> Pred = [](ABaseBlock* Block) { return true; })
{
	return BaseBlockIterator(WorldContextObj, Pred);
}

void ABaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBlock::SetColor(FLinearColor NewColor)
{
	UMaterialInterface* Material = BlockMesh->GetMaterial(0);

	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);
	if (!DynamicMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, BlockMesh);
		BlockMesh->SetMaterial(0, DynamicMaterial);
	}

	DynamicMaterial->SetVectorParameterValue(ColorParameterName, NewColor);
}


