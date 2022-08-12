// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUAmmoPickup.h"
#include "STUHealthPickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUAmmoPickup
{
    GENERATED_BODY()

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
