// Shoot Them Up Game. All Rights Reserved.

#include "AI/Services/STUChangeWeaponService.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "ChangeWeapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!BlackBoard) return;

    const auto Controller = OwnerComp.GetAIOwner();
    if (Controller)
    {
        const auto WeaponComp = STUUtils::GetComponentByClass<USTUAIWeaponComponent>(Controller->GetPawn());
        if (!WeaponComp || FMath::FRand() > Probablity) return;

        WeaponComp->NextWeapon();
    }
}
