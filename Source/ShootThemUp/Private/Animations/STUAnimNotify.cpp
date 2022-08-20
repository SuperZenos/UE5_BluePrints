// Shoot Them Up Game. All Rights Reserved.

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UAnimNotify::Notify(MeshComp, Animation);
    OnNotified.Broadcast(MeshComp);
}
