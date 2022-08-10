// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHudWidget.generated.h"

class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHudWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    UFUNCTION(BlueprintCallable)
    bool bIsPlayerAlive() const;

    UFUNCTION(BlueprintCallable)
    bool bIsPlayerSpectating() const;

protected:
    template<typename T>
    T* GetComponentByClass() const
    {
        auto Player = GetOwningPlayerPawn();
        if (!Player)
            return nullptr;

        auto Component = Player->GetComponentByClass(T::StaticClass());
        if (!Component)
            return nullptr;

        auto WeaponComponent = Cast<T>(Component);
        return WeaponComponent;
    }
};
