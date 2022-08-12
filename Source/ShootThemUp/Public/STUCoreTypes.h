#pragma once

#include "STUCoreTypes.generated.h"
//weapon

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, Display, All);
DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, Display, All);

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

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHair;
};

//health

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

//animation

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

//character

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, Display, All);

//pickup

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, Display, All);
DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, Display, All);
DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, Display, All);