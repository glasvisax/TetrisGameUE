#include "BaseShapeActor.h"

ABaseShapeActor::ABaseShapeActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
}

void ABaseShapeActor::BeginPlay()
{
	Super::BeginPlay();
	
}