#pragma once

#include "CoreMinimal.h"
#include "BaseBlock.h"
#include "GameFramework/Actor.h"
#include <functional>

class ABaseBlock;

class TETRISGAME_API BaseBlockIterator
{
public:
	BaseBlockIterator(UWorld* World, std::function<bool(ABaseBlock*)> Pred);

	ABaseBlock* GetBlock() const;
	operator bool() const;
	BaseBlockIterator& operator++();
	BaseBlockIterator& operator--();
	
private:
	bool IsValidIndex() const;
	TArray<ABaseBlock*> FoundBaseBlocks;
	int32 CurrentIndex;
};