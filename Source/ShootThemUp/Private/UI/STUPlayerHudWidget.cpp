// Shoot Them Up Game. All Rights Reserved


#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"

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
