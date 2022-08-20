// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();
    virtual void Reload() override;
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget"; 

    virtual void MakeShot() override;
    void MakeDamage(FHitResult& HitResult) const;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool bIsVisible);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
