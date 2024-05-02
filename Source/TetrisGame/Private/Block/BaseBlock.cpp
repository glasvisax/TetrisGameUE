#include "BaseBlock.h"
#include "TetrisGameModeBase.h"
#include "BaseBlockIterator.h"
#include "Kismet/GameplayStatics.h"

ABaseBlock::ABaseBlock()
{
	PrimaryActorTick.bCanEverTick = false;
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockMesh");
	SetRootComponent(BlockMesh);
}

void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
}

BaseBlockIterator ABaseBlock::GetBaseBlockIterator(UWorld* WorldContextObj, std::function<bool(ABaseBlock*)> Pred = [](ABaseBlock* Block) -> bool { return true; })
{
	return BaseBlockIterator(WorldContextObj, Pred);
}

void ABaseBlock::SetStaticColor(FLinearColor NewColor)
{
	if (!GetWorld() || !BlockMesh || !BaseMaterial) return;

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, BlockMesh);
	if (!DynamicMaterial) return;

	for (int32 i = 0; i < BlockMesh->GetNumMaterials(); ++i)
	{
		BlockMesh->SetMaterial(i, DynamicMaterial);
	}

	DynamicMaterial->SetVectorParameterValue(ColorParameterName, NewColor);
}

void ABaseBlock::SetUpperCheckered()
{
	if (!EmptyMaterial || !CheckeredMaterial || 
		UpperMaterialIndex > BlockMesh->GetNumMaterials() || 
		UpperMaterialIndex < 0 || !BlockMesh ||
		!GetWorld()) return;

	UMaterialInstanceDynamic* EmptyDynMaterial = UMaterialInstanceDynamic::Create(EmptyMaterial, this);
	UMaterialInstanceDynamic* CheckeredDynMaterial = UMaterialInstanceDynamic::Create(CheckeredMaterial, this);

	if (!EmptyDynMaterial || !CheckeredDynMaterial) return;

	for (int32 i = 0; i < BlockMesh->GetNumMaterials(); ++i)
	{
		BlockMesh->SetMaterial(i, i == UpperMaterialIndex ? CheckeredDynMaterial : EmptyDynMaterial);
	}
}

void ABaseBlock::SetLowerCheckered()
{
	if (!EmptyMaterial || !CheckeredMaterial ||
		LowerMaterialIndex > BlockMesh->GetNumMaterials() ||
		LowerMaterialIndex < 0 || !BlockMesh ||
		!GetWorld()) return;

	UMaterialInstanceDynamic* EmptyDynMaterial = UMaterialInstanceDynamic::Create(EmptyMaterial, this);
	UMaterialInstanceDynamic* CheckeredDynMaterial = UMaterialInstanceDynamic::Create(CheckeredMaterial, this);

	if (!EmptyDynMaterial || !CheckeredDynMaterial) return;

	for (int32 i = 0; i < BlockMesh->GetNumMaterials(); ++i)
	{
		BlockMesh->SetMaterial(i, i == LowerMaterialIndex ? CheckeredDynMaterial : EmptyDynMaterial);
	}
}


