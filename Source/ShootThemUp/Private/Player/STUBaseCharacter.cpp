// Shoot Them Up Game. All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
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
    if (Amount > 0)
        return;
    IsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount > 0)
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
