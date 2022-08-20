// Shoot Them Up Game. All Rights Reserved.

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const auto AimActor = AIPerceptionComponent->GetClosestEnemy();
    SetFocus(AimActor);
}

ASTUAIController::ASTUAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("AIPerceptionComponent");
    SetPerceptionComponent(*AIPerceptionComponent);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AICharacter = Cast<ASTUAICharacter>(GetPawn());
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}
