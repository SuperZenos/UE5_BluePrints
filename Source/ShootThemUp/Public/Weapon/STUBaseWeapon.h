// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();
    bool bIsAmmoEmpty() const;
    bool bIsClipEmpty() const;
    bool bCanReload() const;
    virtual void Reload();
    void ChangeBullets();

    void OnCharacterDeath();

    ACharacter* GetCharacter() const;
    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;

    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    FAmmoData DefaultAmmo{0, 28, 300, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    float ShootDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    float BodyDamage = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    float HeadDamage = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    float RevolutionsPerMinute = 600.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    float BulletSpread = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    FTimerHandle ShotTimerHandle;

    virtual void BeginPlay() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    virtual void MakeShot();
    virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    virtual bool bIsHitValid(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();
    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
