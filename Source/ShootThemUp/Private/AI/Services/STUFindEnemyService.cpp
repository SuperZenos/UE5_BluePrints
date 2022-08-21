// Shoot Them Up Game. All Rights Reserved.

#include "AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUHealthComponent.h"
#include "Player/STUBaseCharacter.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!BlackBoard) return;

    const auto Controller = OwnerComp.GetAIOwner();
    const auto PerceptionComponent = STUUtils::GetComponentByClass<USTUAIPerceptionComponent>(Controller);
    if (Controller && PerceptionComponent)
    {
        const auto CurrentEnemy = Cast<ASTUBaseCharacter>(BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
        if (!CurrentEnemy || !PerceptionComponent->bIsInPerceivedActors(CurrentEnemy) || !CurrentEnemy->bIsAlive())
            BlackBoard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
    }
}
