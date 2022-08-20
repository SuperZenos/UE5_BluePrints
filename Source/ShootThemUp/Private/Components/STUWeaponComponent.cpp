// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "Weapon/STURifleWeapon.h"
#include "Weapon/STULauncherWeapon.h"
#include "Animations/STUEquipStartNotify.h"
#include "Animations/STUEquipAnimNotify.h"
#include "Animations/STUReloadStartNotify.h"
#include "Animations/STUReloadAnimNotify.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponClasses.Num() <= 2, TEXT("Our character can hold only less than 2 weapon items."));

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->OnCharacterDeath();
    }
    Weapons.Empty();
}

void USTUWeaponComponent::SpawnWeapons()
{
    if (!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon) continue;
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);
        AttachWeaponToSocket(Character->GetMesh(), Weapon, WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(USceneComponent* SceneComponent, ASTUBaseWeapon* Weapon, const FName SocketName)
{
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(Character->GetMesh(), CurrentWeapon, WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[CurrentWeaponIndex];
    AttachWeaponToSocket(Character->GetMesh(), CurrentWeapon, WeaponAttachSocketName);

    bEquipAnimInProgress = true;

    Character->PlayEquipAnimMontage();
}

void USTUWeaponComponent::InitAnimations()
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player) return;

    auto EquipStartNotify = Player->FindAnimNotifyByClass<USTUEquipStartNotify>(EAnimMontageName ::EquipAnimMontage);
    if (EquipStartNotify)
    {
        EquipStartNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipStart);
    }
    else
        UE_LOG(LogWeaponComponent, Error, TEXT("EquipAnimMontage is forgotten to set."));

    auto EquipFinishedNotify = Player->FindAnimNotifyByClass<USTUEquipAnimNotify>(EAnimMontageName ::EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
        UE_LOG(LogWeaponComponent, Error, TEXT("EquipAnimMontage is forgotten to set."));

    auto RifleReloadStartNotify = Player->FindAnimNotifyByClass<USTUReloadStartNotify>(EAnimMontageName ::RifleReloadAnimMontage);
    if (RifleReloadStartNotify)
    {
        RifleReloadStartNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadStart);
    }
    else
        UE_LOG(LogWeaponComponent, Error, TEXT("RifleReloadAnimMontage is forgotten to set."));

    auto RifleReloadFinishedNotify = Player->FindAnimNotifyByClass<USTUReloadAnimNotify>(EAnimMontageName ::RifleReloadAnimMontage);
    if (RifleReloadFinishedNotify)
    {
        RifleReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
    else
        UE_LOG(LogWeaponComponent, Error, TEXT("RifleReloadAnimMontage is forgotten to set."));

    auto LauncherReloadStartNotify = Player->FindAnimNotifyByClass<USTUReloadStartNotify>(EAnimMontageName ::LauncherReloadAnimMontage);
    if (LauncherReloadStartNotify)
    {
        LauncherReloadStartNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadStart);
    }
    else
        UE_LOG(LogWeaponComponent, Error, TEXT("LauncherReloadAnimMontage is forgotten to set."));

    auto LauncherReloadFinishedNotify = Player->FindAnimNotifyByClass<USTUReloadAnimNotify>(EAnimMontageName ::LauncherReloadAnimMontage);
    if (LauncherReloadFinishedNotify)
    {
        LauncherReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
    else
        UE_LOG(LogWeaponComponent, Error, TEXT("LauncherReloadAnimMontage is forgotten to set."));
}

void USTUWeaponComponent::OnEquipStart(USkeletalMeshComponent* MeshComp)
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || !(MeshComp->GetOwner() == Player)) return;

    bEquipAnimInProgress = true;
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || !(MeshComp->GetOwner() == Player)) return;

    bEquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadStart(USkeletalMeshComponent* MeshComp)
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || !(MeshComp->GetOwner() == Player)) return;

    bReloadAnimInProgress = true;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || !(MeshComp->GetOwner() == Player)) return;

    ChangeBullets();
    bReloadAnimInProgress = false;
}

bool USTUWeaponComponent::bCanDoAction() const
{
    return !bEquipAnimInProgress && !bReloadAnimInProgress && CurrentWeapon;
}

void USTUWeaponComponent::StartFire()
{
    if (!bCanDoAction()) return;

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if (!bCanDoAction()) return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload()
{
    if (!bCanDoAction()) return;

    CurrentWeapon->Reload();
}

void USTUWeaponComponent::ChangeBullets()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->ChangeBullets();
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 BulletsAmount)
{
    for (auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryAddAmmo(BulletsAmount);
        }
    }
    return false;
}
