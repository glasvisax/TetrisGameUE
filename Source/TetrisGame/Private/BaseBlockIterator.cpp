#include "BaseBlockIterator.h"
#include "Kismet/GameplayStatics.h"

BaseBlockIterator::BaseBlockIterator(UWorld* World, std::function<bool(ABaseBlock*)> Pred = [](ABaseBlock* Block) { return true; }) : CurrentIndex(0)
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

ABaseBlock* BaseBlockIterator::Get() const
{
    return IsValidIndex(CurrentIndex) ? Cast<ABaseBlock>(FoundBaseBlocks[CurrentIndex]) : nullptr;
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

bool BaseBlockIterator::IsValidIndex(int32 Index) const
{
    return (Index >= 0) && (Index < FoundBaseBlocks.Num()) && FoundBaseBlocks[Index] != nullptr;
}


