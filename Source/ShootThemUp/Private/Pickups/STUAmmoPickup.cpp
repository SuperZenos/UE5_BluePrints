// Shoot Them Up Game. All Rights Reserved


#include "Pickups/STUAmmoPickup.h"

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (PlayerPawn)
    {
        UE_LOG(LogAmmoPickup, Display, TEXT("Ammo pickup was taken"));
        return true;
    }
    return false;
}
