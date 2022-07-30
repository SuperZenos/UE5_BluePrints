// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
    // Set projectile params
    
    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
    //GetWorld()->SpawnActor<ASTUProjectile>(ProjectileClass, SpawnTransform);
}
