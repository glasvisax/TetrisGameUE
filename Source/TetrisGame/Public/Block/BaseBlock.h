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

	void SetStaticColor(FLinearColor NewColor);

	void SetUpperCheckered();

	void SetLowerCheckered();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	FName ColorParameterName = "BaseColor";

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UMaterial* BaseMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UMaterial* EmptyMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UMaterial* CheckeredMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	int32 UpperMaterialIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	int32 LowerMaterialIndex = 0;

};

