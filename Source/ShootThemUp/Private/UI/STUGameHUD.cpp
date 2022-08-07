// Shoot Them Up Game. All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCrossHair();
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

void ASTUGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    FLinearColor LineColor = FLinearColor::Red;
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min + HalfLineSize, Center.Max, Center.Min, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max + HalfLineSize, Center.Min, Center.Max, LineColor, LineThickness);
}
