// Shoot Them Up Game. All Rights Reserved


#include "Pickups/STUHealthPickup.h"

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (PlayerPawn)
    {
        UE_LOG(LogHealthPickup, Display, TEXT("Health pickup was taken"));
        return true;
    }
    return false;
}