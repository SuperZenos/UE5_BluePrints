// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();
    virtual void Reload() override;

protected:
    virtual void MakeShot() override;
    void MakeDamage(FHitResult& HitResult) const;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
};
