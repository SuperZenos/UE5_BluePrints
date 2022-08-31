// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(DefaultAmmo.BulletsInClip >= 0, TEXT("Bullets count count't be less than zero."));
    checkf(DefaultAmmo.CapacityOfClip >= 0, TEXT("Capacity of clip count't be less than zero."));
    checkf(DefaultAmmo.SpareBullets >= 0, TEXT("Spare bullets count't be less than zero."));

    DefaultAmmo.BulletsInClip = DefaultAmmo.CapacityOfClip;
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
    if (bIsClipEmpty() && bCanReload())
    {
        Reload();
        return;
    }

    MakeShot();
    float TimeBetweenShots = 60.0f / RevolutionsPerMinute;
    if (!GetWorldTimerManager().IsTimerActive(ShotTimerHandle))
        GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTUBaseWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTUBaseWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTUBaseWeapon::MakeShot() {}

void ASTUBaseWeapon::OnCharacterDeath()
{
    StopFire();
    Destroy();
}

ACharacter* ASTUBaseWeapon::GetCharacter() const
{
    auto Player = Cast<ACharacter>(GetOwner());
    return Player;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = GetCharacter();
    if (!Player) return nullptr;
    const auto Controller = Player->GetController<APlayerController>();
    return Controller;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto STUCharacter = Cast<ASTUBaseCharacter>(GetOwner());
    if (!STUCharacter) return false;

    if (STUCharacter->IsPlayerControlled())
    {
        auto Controller = GetPlayerController();
        if (!Controller) return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = ViewLocation + ShootDirection * ShootDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

bool ASTUBaseWeapon::bIsHitValid(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FVector TraceDirectionVector = TraceEnd - TraceStart;
    TraceDirectionVector.Normalize();

    FVector ShootDirectionVector = HitResult.Location - GetMuzzleWorldLocation();
    ShootDirectionVector.Normalize();

    float ShootSingle = FMath::Acos(FVector::DotProduct(ShootDirectionVector, TraceDirectionVector));
    if (FMath::RadiansToDegrees(ShootSingle) < 60) return true;
    return false;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    CurrentAmmo.BulletsInClip -= 1;
    LogAmmo();

    if (bIsClipEmpty() && bCanReload()) Reload();
}

bool ASTUBaseWeapon::bIsAmmoEmpty() const
{
    return !CurrentAmmo.bInfinite && bIsClipEmpty() && CurrentAmmo.SpareBullets == 0;
}

bool ASTUBaseWeapon::bIsClipEmpty() const
{
    return CurrentAmmo.BulletsInClip == 0;
}

bool ASTUBaseWeapon::bCanReload() const
{
    return (CurrentAmmo.bInfinite || CurrentAmmo.SpareBullets != 0) && CurrentAmmo.BulletsInClip != CurrentAmmo.CapacityOfClip;
}

void ASTUBaseWeapon::Reload()
{
    StopFire();
    UE_LOG(LogBaseWeapon, Display, TEXT("Reload"));
}

bool ASTUBaseWeapon::TryToGetAmmoPickup(int32 BulletsAmount)
{
    if (CurrentAmmo.bInfinite || CurrentAmmo.SpareBullets == DefaultAmmo.SpareBullets) return false;

    CurrentAmmo.SpareBullets = FMath::Min(CurrentAmmo.SpareBullets + BulletsAmount, DefaultAmmo.SpareBullets);
    return true;
}

void ASTUBaseWeapon::ChangeBullets()
{
    if (CurrentAmmo.bInfinite == true)
        CurrentAmmo.BulletsInClip = CurrentAmmo.CapacityOfClip;
    else
    {
        int32 ReloadBullets = FMath::Min(CurrentAmmo.CapacityOfClip - CurrentAmmo.BulletsInClip, CurrentAmmo.SpareBullets);
        CurrentAmmo.BulletsInClip = CurrentAmmo.BulletsInClip + ReloadBullets;
        CurrentAmmo.SpareBullets = CurrentAmmo.SpareBullets - ReloadBullets;
    }
    LogAmmo();
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.BulletsInClip) + "/";
    AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.SpareBullets);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,  //
        WeaponMesh,                                                //
        MuzzleSocketName,                                          //
        FVector::ZeroVector,                                       //
        FRotator::ZeroRotator,                                     //
        EAttachLocation::Type::SnapToTarget, true);
}

void ASTUBaseWeapon::PlayShootCameraShake()
{
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    //auto CameraShake = Controller->PlayerCameraManager->StartCameraShake(ShootCameraShake);
}
