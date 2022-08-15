// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Player/STUBaseCharacter.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
    BulletSpread = 1.5f;
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || bIsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    if (HitResult.bBlockingHit && bIsHitValid(HitResult, TraceStart, TraceEnd))
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::White, false, 3.0f, 0);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Red, false, 5.0f);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::White, false, 3.0f, 0);
    }

    DecreaseAmmo();
}

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult) const
{
    auto* HitActor = HitResult.GetActor();
    if (!HitActor) return;

    if (HitResult.BoneName.ToString() == "b_head")
        HitActor->TakeDamage(HeadDamage, {}, GetPlayerController(), GetOwner());
    else
        HitActor->TakeDamage(BodyDamage, {}, GetPlayerController(), GetOwner());
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const float HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = ViewLocation + ShootDirection * ShootDistance;
    return true;
}

void ASTURifleWeapon::Reload()
{
    if (!bCanReload()) return;

    Super::Reload();

    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player) return;

    Player->PlayRifleReloadAnimMontage();
}
