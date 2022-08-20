// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToGetHealthPickup(float HealthAmount)
{
    if (Health == MaxHealth)
        return false;
    else
    {
        SetHealth(FMath::Clamp(Health + HealthAmount, 0, MaxHealth));
        return true;
    }
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || bIsDead() || !GetWorld()) return;

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    SetHealth(Health - Damage);

    if (bIsDead())
    {
        OnDeath.Broadcast();
    }

    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdata, HealUpdateTime, true, HealDelay);
    }

    PlayCameraShake();
}

void USTUHealthComponent::HealUpdata()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (bIsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller||!Controller->PlayerCameraManager) return;
    
    Controller->PlayerCameraManager->StartCameraShake(DamageCameraShake);
}
