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

void ASTUBaseWeapon::StartFire() {}

void ASTUBaseWeapon::StopFire() {}

void ASTUBaseWeapon::MakeShot() {}

void ASTUBaseWeapon::OnCharacterDeath()
{
    SetLifeSpan(3.0f);
    StopFire();
}

ACharacter* ASTUBaseWeapon::GetCharacter() const
{
    auto Player = Cast<ACharacter>(GetOwner());
    return Player;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = GetCharacter();
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
    const FVector ShootDirection = ViewRotation.Vector();
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

    float ShootSingle = FMath::Acos(FVector::DotProduct(ShootDirectionVector, TraceDirectionVector));
    if (FMath::RadiansToDegrees(ShootSingle) < 60)
        return true;
    return false;
}