// Shoot Them Up Game. All Rights Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "Animations/STUEquipAnimNotify.h"

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
    ACharacter* Character = Cast<ACharacter>(GetOwner());
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

    PlayEquipAnimMontage();
}

void USTUWeaponComponent::PlayEquipAnimMontage()
{
    auto Player = Cast<ASTUBaseCharacter>(CurrentWeapon->GetCharacter());
    if (Player)
    {
        Player->PlayEquipAnimMontage();
    }
}

void USTUWeaponComponent::InitAnimations()
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player)
        return;

    auto EquipAnimMontage = Player->GetEquipAnimMontage();
    if (!EquipAnimMontage)
        return;

    const auto NotifyEvents = EquipAnimMontage->Notifies;
    for (auto NotifyEvent : NotifyEvents)
    {
        auto EquipFinishedNotify = Cast<USTUEquipAnimNotify>(NotifyEvent.Notify);
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    bEquipAnimInProgress = false;

    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player || MeshComp->GetOwner() == Player)
        return;
}

bool USTUWeaponComponent::bCanEquip() const
{
    return !bEquipAnimInProgress;
}

bool USTUWeaponComponent::bCanFire() const
{
    return !bEquipAnimInProgress && CurrentWeapon;
}

void USTUWeaponComponent::StartFire()
{
    if (!bCanFire())
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
    if (!bCanEquip())
        return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}