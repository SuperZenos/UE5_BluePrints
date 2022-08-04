// Shoot Them Up Game. All Rights Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "Animations/STUEquipAnimNotify.h"
#include "Weapon/STURifleWeapon.h"
#include "Weapon/STULauncherWeapon.h"
#include "Animations/STUReloadAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, Display, All);

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

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
    if (!GetWorld())
        return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon)
            continue;
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);
        AttachWeaponToSocket(Character->GetMesh(), Weapon, WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(USceneComponent* SceneComponent, ASTUBaseWeapon* Weapon, const FName SocketName)
{
    if (!Weapon || !SceneComponent)
        return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Character)
        return;

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
    if (!Player)
        return;

    auto EquipFinishedNotify = Player->FindAnimNotifyByClass<USTUEquipAnimNotify>(EAnimMontageName ::EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }

    auto RifleReloadFinishedNotify = Player->FindAnimNotifyByClass<USTUReloadAnimNotify>(EAnimMontageName ::RifleReloadAnimMontage);
    if (RifleReloadFinishedNotify)
    {
        RifleReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }

    auto LauncherReloadFinishedNotify = Player->FindAnimNotifyByClass<USTUReloadAnimNotify>(EAnimMontageName ::LauncherReloadAnimMontage);
    if (LauncherReloadFinishedNotify)
    {
        LauncherReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || !(MeshComp->GetOwner() == Player))
        return;

    bEquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || !(MeshComp->GetOwner() == Player))
        return;

    bReloadAnimInProgress = false;
}

bool USTUWeaponComponent::bCanDoAction() const
{
    return !bEquipAnimInProgress && !bReloadAnimInProgress && CurrentWeapon;
}

void USTUWeaponComponent::StartFire()
{
    if (!bCanDoAction())
        return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if (!bCanDoAction())
        return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload()
{
    if (!CurrentWeapon || !bCanDoAction())
        return;

    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player)
        return;

    bReloadAnimInProgress = true;
    CurrentWeapon->Reload();
}