// Shoot Them Up Game. All Rights Reserved.

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    UAnimNotify::Notify(MeshComp, Animation);
    OnNotified.Broadcast(MeshComp);
}
