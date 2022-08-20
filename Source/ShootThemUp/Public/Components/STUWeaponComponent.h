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

    void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload();
    void ChangeBullets();

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 BulletsAmount);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachSocketName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmory";

    void AttachWeaponToSocket(USceneComponent* SceneComponent, ASTUBaseWeapon* Weapon, const FName SocketName);

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

private:
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;

    void SpawnWeapons();
    void EquipWeapon(int32 WeaponIndex);
    void InitAnimations();

    void OnEquipStart(USkeletalMeshComponent* MeshComp);
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadStart(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool bCanDoAction() const;
};
