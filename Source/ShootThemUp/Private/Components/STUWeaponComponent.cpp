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
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    if (!Player)
        return;

    if (MeshComp->GetOwner()==Player)
        UE_LOG(LogWeaponComponent, Display, TEXT("EquipFinished"));
}

void USTUWeaponComponent::StartFire()
{
    if (!CurrentWeapon)
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
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}