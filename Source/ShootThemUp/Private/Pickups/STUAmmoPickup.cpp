// Shoot Them Up Game. All Rights Reserved

#include "Pickups/STUAmmoPickup.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn)
        return false;

    auto HealthComponent = STUUtils::GetComponentByClass<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->bIsDead())
        return false;

    auto WeaponComponent = STUUtils::GetComponentByClass<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryAddAmmo(WeaponType, BulletsAmount);
}
