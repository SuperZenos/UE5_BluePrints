// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUPlayerHudWidget::GetHealthPercent() const
{
    auto HealthComponent = STUUtils::GetComponentByClass<USTUHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    auto WeaponComponent = STUUtils::GetComponentByClass<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent && WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHudWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    auto WeaponComponent = STUUtils::GetComponentByClass<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent && WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool USTUPlayerHudWidget::bIsPlayerAlive() const
{
    auto HealthComponent = STUUtils::GetComponentByClass<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->bIsDead();
}

bool USTUPlayerHudWidget::bIsPlayerSpectating() const
{
    auto Player = GetOwningPlayer();
    return Player && Player->GetStateName() == NAME_Spectating;
}
