// Shoot Them Up Game. All Rights Reserved.

#include "AI/Services/STUFireService.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto Controller = OwnerComp.GetAIOwner();
    const auto Pawn = Controller->GetPawn();
    const auto WeaponComp = STUUtils::GetComponentByClass<USTUWeaponComponent>(Pawn);
    if (!Controller || !Pawn || !WeaponComp) return;

    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!BlackBoard || !BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName))
    {
        WeaponComp->StopFire();
        return;
    }
    WeaponComp->StartFire();
}
