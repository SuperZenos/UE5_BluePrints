// Shoot Them Up Game. All Rights Reserved.

#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn) return false;

    auto HealthComponent = STUUtils::GetComponentByClass<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->bIsDead()) return false;

    return HealthComponent->TryToGetHealthPickup(HealthAmount);
}