// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceptionActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceptionActors);
    if (PerceptionActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Cast<APawn>(Controller->GetPawn());
    if (!Pawn) return nullptr;

    AActor* BestActor = nullptr;
    float BestDistance = MAX_FLT;
    for (const auto PerceptionActor : PerceptionActors)
    {
        const auto HealthComponent = STUUtils::GetComponentByClass<USTUHealthComponent>(PerceptionActor);
        if (HealthComponent && !HealthComponent->bIsDead())
        {
            const auto CurrentDistance = (PerceptionActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestActor = PerceptionActor;
            }
        }
    }

    return BestActor;
}

bool USTUAIPerceptionComponent::bIsInPerceivedActors(AActor* const InActor) const
{
    if (!InActor) return false;

    TArray<AActor*> PerceptionActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceptionActors);
    return PerceptionActors.Contains(InActor);
}
