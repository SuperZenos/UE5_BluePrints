// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUCoreTypes.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUBaseWeapon;
class USTUWeaponComponent;

enum EAnimMontageName
{
    DeathAnimMontage,
    EquipAnimMontage,
    RifleReloadAnimMontage,
    LauncherReloadAnimMontage
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);
    void PlayEquipAnimMontage();
    void PlayRifleReloadAnimMontage();
    void PlayLauncherReloadAnimMontage();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* RifleReloadAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* LauncherReloadAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const { return PressRunning && IsMovingForward && !GetVelocity().IsZero(); }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    bool PressRunning = false;
    bool IsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void RunningStart();
    void RunningEnd();

    void OnDeath();
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

public:
    template <typename T>
    T* FindAnimNotifyByClass(EAnimMontageName AnimMontageName)
    {
        TArray<FAnimNotifyEvent> AnimNotifyEvents;
        switch (AnimMontageName)
        {
            case EAnimMontageName::DeathAnimMontage:
                if (!DeathAnimMontage) return nullptr;
                AnimNotifyEvents = DeathAnimMontage->Notifies;
                break;

            case EAnimMontageName::EquipAnimMontage:
                if (!EquipAnimMontage) return nullptr;
                AnimNotifyEvents = EquipAnimMontage->Notifies;
                break;

            case EAnimMontageName::RifleReloadAnimMontage:
                if (!RifleReloadAnimMontage) return nullptr;
                AnimNotifyEvents = RifleReloadAnimMontage->Notifies;
                break;

            case EAnimMontageName::LauncherReloadAnimMontage:
                if (!LauncherReloadAnimMontage) return nullptr;
                AnimNotifyEvents = LauncherReloadAnimMontage->Notifies;
                break;
        }

        for (auto NotifyEvent : AnimNotifyEvents)
        {
            auto ActionFinishedNotify = Cast<T>(NotifyEvent.Notify);
            if (ActionFinishedNotify)
            {
                return ActionFinishedNotify;
            }
        }

        return nullptr;
    }
};
