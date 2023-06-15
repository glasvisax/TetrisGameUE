#include "BaseShapeActor.h"

ABaseShapeActor::ABaseShapeActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRootComponent);
}

void ABaseShapeActor::BeginPlay()
{
	Super::BeginPlay();
	
}