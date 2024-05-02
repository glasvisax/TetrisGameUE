#include "BaseBlockIterator.h"
#include "Kismet/GameplayStatics.h"

BaseBlockIterator::BaseBlockIterator(UWorld* World, std::function<bool(ABaseBlock*)> Pred = [](ABaseBlock* Block) -> bool { return true; }) : CurrentIndex(0)
{
    if (World)
    {
        TArray<AActor*> Actors;
        TArray<ABaseBlock*> Blocks;
        UGameplayStatics::GetAllActorsOfClass(World, ABaseBlock::StaticClass(), Actors);
        for (const auto i : Actors) 
        {
            ABaseBlock* BaseBlock = Cast<ABaseBlock>(i);

            if (BaseBlock)
            {
                Blocks.Add(BaseBlock);
            }
        }
        FoundBaseBlocks = Blocks.FilterByPredicate(Pred);
    }
}

ABaseBlock* BaseBlockIterator::GetBlock() const
{
    return IsValidIndex() ? Cast<ABaseBlock>(FoundBaseBlocks[CurrentIndex]) : nullptr;
}

BaseBlockIterator::operator bool() const
{
    return IsValidIndex();
}

BaseBlockIterator& BaseBlockIterator::operator++()
{
    ++CurrentIndex;
    return *this;
}

BaseBlockIterator& BaseBlockIterator::operator--()
{
    --CurrentIndex;
    return *this;
}

bool BaseBlockIterator::IsValidIndex() const
{
    return (CurrentIndex >= 0) && (CurrentIndex < FoundBaseBlocks.Num()) && FoundBaseBlocks[CurrentIndex] != nullptr;
}


