// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetShotDirection(const FVector& Direction);

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Component")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Component")
    UProjectileMovementComponent* MovementComponent;

    virtual void BeginPlay() override;

private:
    FVector ShotDirection = FVector::ZeroVector;
};
