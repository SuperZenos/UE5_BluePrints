// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    if (!CurrentWeapon || !bCanDoAction()) return;
    if (bIsAmmoEmpty())
        NextWeapon();
    else
        Super::StartFire();
}

void USTUAIWeaponComponent::NextWeapon()
{
    if (!bCanDoAction()) return;

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->bIsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }
    if (CurrentWeaponIndex == NextIndex) return;
    CurrentWeaponIndex = NextIndex;
    EquipWeapon(CurrentWeaponIndex);
}
