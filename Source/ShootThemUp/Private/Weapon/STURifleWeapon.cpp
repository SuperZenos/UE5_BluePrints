// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Player/STUBaseCharacter.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit && bIsHitValid(HitResult, TraceStart, TraceEnd))
    {
        TraceFXEnd = HitResult.Location;
        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }

    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);

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

void ASTURifleWeapon::StartFire()
{
    Super::StartFire();
    InitMuzzleFX();
}

void ASTURifleWeapon::StopFire()
{
    Super::StopFire();
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent) MuzzleFXComponent = SpawnMuzzleFX();
    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool bIsVisible)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!bIsVisible);
        MuzzleFXComponent->SetVisibility(bIsVisible, true);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
