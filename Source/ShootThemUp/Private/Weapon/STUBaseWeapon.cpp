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

void ASTUBaseWeapon::Fire()
{
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire"));

    MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld())
        return;

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller)
        return;

    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FTransform SoketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

    const FVector TraceStart = ViewLocation;              // SoketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector(); // SoketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * ShootDistance;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Player);


    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(
        HitResult, SoketTransform.GetLocation(), TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), SoketTransform.GetLocation(), HitResult.ImpactPoint, FColor::White, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Red, false, 5.0f);

        UE_LOG(LogSTUBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
    }
    else
    {
        DrawDebugLine(GetWorld(), SoketTransform.GetLocation(), TraceEnd, FColor::White, false, 3.0f, 0, 3.0f);
    }
}
