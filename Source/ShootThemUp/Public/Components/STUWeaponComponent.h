// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    virtual void StartFire();
    virtual void StopFire();
    virtual void NextWeapon();
    virtual void Reload();
    bool bCanDoAction() const;

    bool bIsFiring() const { return bFireFlag; }

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

    bool bIsAmmoEmpty() const;

    bool TryToGetAmmoPickup(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 BulletsAmount);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachSocketName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmory";

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

    void EquipWeapon(int32 WeaponIndex);
    void SpawnWeapons();
    void AttachWeaponToSocket(USceneComponent* SceneComponent, ASTUBaseWeapon* Weapon, const FName SocketName);

private:
    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;
    bool bFireFlag = false;
    
    void InitAnimations();

    void OnEquipStart(USkeletalMeshComponent* MeshComp);
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadStart(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    void ChangeBullets();
};
