// Shoot Them Up Game. All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();
    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass, "PlayerHUDWidget");
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}