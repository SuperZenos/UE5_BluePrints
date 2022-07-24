// Shoot Them Up Game. All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseCharacter, Display, All)

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 50.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    HealthTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
    HealthTextComponent->SetTextRenderColor(FColor::Red);
    HealthTextComponent->SetOwnerNoSee(true);
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

    SpawnWeapon();
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto Health = HealthComponent->GetHealth();
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &Super::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &Super::AddControllerYawInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Running", IE_Pressed, this, &ASTUBaseCharacter::RunningStart);
    PlayerInputComponent->BindAction("Running", IE_Released, this, &ASTUBaseCharacter::RunningEnd);
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;
    FVector ForwardVector = GetActorForwardVector();
    FVector VelocityVector = GetVelocity().GetSafeNormal();
    float DotNum = FVector::DotProduct(ForwardVector, VelocityVector);
    FVector CrossProduct = FVector::CrossProduct(ForwardVector, VelocityVector);
    float Direction = FMath::Acos(DotNum);
    Direction = CrossProduct.IsZero() ? Direction : FMath::Sign(CrossProduct.Z) * Direction;
    return FMath::RadiansToDegrees(Direction);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    if (Amount == 0)
        return;
    IsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0)
        return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::RunningStart()
{
    PressRunning = true;
}

void ASTUBaseCharacter::RunningEnd()
{
    PressRunning = false;
}

void ASTUBaseCharacter::OnDeath()
{
    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);
    float LifeSpan = GetLifeSpan();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = GetVelocity().Z;
    UE_LOG(LogSTUBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (-FallVelocityZ < LandedDamageVelocity.X)
        return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, -FallVelocityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTUBaseCharacter::SpawnWeapon()
{
    if (!GetWorld())
        return;
    const auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (Weapon)
    {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
        Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponPoint" );
    }
}
