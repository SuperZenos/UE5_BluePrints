// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 BulletsInClip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 CapacityOfClip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
    int32 SpareBullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bInfinite;

    FAmmoData()
    {
        BulletsInClip = 0;
        CapacityOfClip = 0;
        SpareBullets = 0;
        bInfinite = false;
    }
    FAmmoData(int32 Bullets, int32 Capacity, int32 Total, bool Infinite)
    {
        BulletsInClip = Bullets;
        CapacityOfClip = Capacity;
        SpareBullets = Total;
        bInfinite = Infinite;
    }
};

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

    void OnCharacterDeath();

    ACharacter* GetCharacter() const;
    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponParams")
    FAmmoData DefaultAmmo{28, 28, 300, false};

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
