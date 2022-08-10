// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUPlayerHudWidget::GetHealthPercent() const
{
    auto HealthComponent = GetComponentByClass<USTUHealthComponent>();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    auto WeaponComponent = GetComponentByClass<USTUWeaponComponent>();
    return WeaponComponent && WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHudWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    auto WeaponComponent = GetComponentByClass<USTUWeaponComponent>();
    return WeaponComponent && WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool USTUPlayerHudWidget::bIsPlayerAlive() const
{
    auto HealthComponent = GetComponentByClass<USTUHealthComponent>();
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHudWidget::bIsPlayerSpectating() const
{
    auto Player = GetOwningPlayer();
    return Player && Player->GetStateName() == NAME_Spectating;
}
