// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

float USTUPlayerHudWidget::GetHealthPercent() const
{
    auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.0f;

    auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    if (!Component)
        return 0.0f;

    auto HealthComponent = Cast<USTUHealthComponent>(Component);
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHudWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

USTUWeaponComponent* USTUPlayerHudWidget::GetWeaponComponent() const
{
    auto Player = GetOwningPlayerPawn();
    if (!Player)
        return nullptr;

    auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    if (!Component)
        return nullptr;

    auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
    return WeaponComponent;
}
