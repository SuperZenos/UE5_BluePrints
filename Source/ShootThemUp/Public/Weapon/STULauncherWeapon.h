// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class ASTUProjectile;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "WeaponParams")
    TSubclassOf<ASTUProjectile> ProjectileClass;

    virtual void MakeShot() override;
};
