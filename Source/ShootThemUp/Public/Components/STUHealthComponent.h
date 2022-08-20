// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool bIsDead() const { return Health == 0; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() { return Health / MaxHealth; }

    float GetHealth() const { return Health; }

    bool TryToGetHealthPickup(float HealthAmount);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base", meta = (Clampmin = "0.0", Clampmax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal",
        meta = (Clampmin = "0.0", Clampmax = "10.0", EditCondition = "AutoHeal"))
    float HealUpdateTime = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal",
        meta = (Clampmin = "0.0", Clampmax = "10.0", EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal",
        meta = (Clampmin = "0.0", Clampmax = "100.0", EditCondition = "AutoHeal"))
    float HealModifier = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> DamageCameraShake;

    float HealTimeLeft = HealDelay;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;
    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void HealUpdata();
    void SetHealth(float NewHealth);

    void PlayCameraShake();
};
