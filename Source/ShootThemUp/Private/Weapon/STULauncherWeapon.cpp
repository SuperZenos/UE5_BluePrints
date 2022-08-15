// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Player/STUBaseCharacter.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld() || bIsAmmoEmpty()) return;

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform, this);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
}

void ASTULauncherWeapon::Reload()
{
    if (!bCanReload()) return;

    Super::Reload();

    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player) return;

    Player->PlayLauncherReloadAnimMontage();
}