#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBlockIterator.h"
#include "BaseBlock.generated.h"
class BaseBlockIterator;
UCLASS()
class TETRISGAME_API ABaseBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBlock();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* BlockMesh;
	
public:	
	static BaseBlockIterator GetBaseBlockIterator(UWorld* WorldContextObj, std::function<bool(ABaseBlock*)> Pred);

	virtual void Tick(float DeltaTime) override;

	void SetColor(FLinearColor NewColor);

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	FName ColorParameterName = "BaseColor";
private:

};

