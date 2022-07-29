// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseWeapon, Display, All)

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
}

void ASTUBaseWeapon::StartFire()
{
    MakeShot();
    float TimerBetweenShots = 60.0f / RevolutionsPerMinute;
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTUBaseWeapon::MakeShot, TimerBetweenShots, true);
}

void ASTUBaseWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTUBaseWeapon::OnCharacterDeath()
{
    SetLifeSpan(3.0f);
    StopFire();
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld())
        return;

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    if (HitResult.bBlockingHit && bIsHitValid(HitResult, TraceStart, TraceEnd))
    {
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("BoneName: %s"), *HitResult.BoneName.ToString());
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::White, false, 3.0f, 0);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Red, false, 5.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::White, false, 3.0f, 0);
    }
}

ACharacter* ASTUBaseWeapon::GetOwner() const
{
    auto Player = Cast<ACharacter>(Super::GetOwner());
    return Player;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = GetOwner();
    if (!Player)
        return nullptr;
    const auto Controller = Player->GetController<APlayerController>();
    return Controller;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    auto Controller = GetPlayerController();
    if (!Controller)
        return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
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
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const float HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = ViewLocation + ShootDirection * ShootDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(FHitResult& HitResult) const
{
    auto* HitActor = HitResult.GetActor();
    if (!HitActor)
        return;

    if (HitResult.BoneName.ToString() == "b_head")
        HitActor->TakeDamage(HeadDamage, {}, GetPlayerController(), GetOwner());
    else
        HitActor->TakeDamage(BodyDamage, {}, GetPlayerController(), GetOwner());
}

bool ASTUBaseWeapon::bIsHitValid(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FVector TraceDirectionVector = TraceEnd - TraceStart;
    TraceDirectionVector.Normalize();

    FVector ShootDirectionVector = HitResult.Location - GetMuzzleWorldLocation();
    ShootDirectionVector.Normalize();

    auto Player = GetOwner();
    if (!Player)
        return false;

    float ShootSingle = FMath::Acos(FVector::DotProduct(ShootDirectionVector, TraceDirectionVector));
    if (FMath::RadiansToDegrees(ShootSingle) < 60)
        return true;
    return false;
}