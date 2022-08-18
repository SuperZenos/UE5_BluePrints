// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetShotDirection(const FVector& Direction);

    APlayerController* ASTUProjectile::GetPlayerController() const;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Component")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UNiagaraComponent* NiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectileParams")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectileParams")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectileParams")
    bool bDoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectileParams")
    float LifeSpan = 5.0f;

    virtual void BeginPlay() override;

private:
    FVector ShotDirection = FVector::ZeroVector;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
