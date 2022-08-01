// Shoot Them Up Game. All Rights Reserved

#include "Animations/STUEquipAnimNotify.h"

void USTUEquipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UAnimNotify::Notify(MeshComp, Animation);
    OnNotified.Broadcast(MeshComp);
}
