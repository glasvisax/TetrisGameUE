#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBlock.generated.h"

UCLASS()
class TETRISGAME_API ABaseBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBlock();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* BlockMesh;

public:	
	virtual void Tick(float DeltaTime) override;

};

