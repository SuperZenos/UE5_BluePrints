// Shoot Them Up Game. All Rights Reserved.

#include "AI/Decorators/STUHealthPercentDecorator.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HealthComponent = STUUtils::GetComponentByClass<USTUHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->bIsDead()) return false;

    return HealthComponent->GetHealthPercent() < HealthPercent; 
}
