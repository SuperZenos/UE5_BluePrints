// Shoot Them Up Game. All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}
