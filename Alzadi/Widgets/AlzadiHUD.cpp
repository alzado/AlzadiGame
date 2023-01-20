// Fill out your copyright notice in the Description page of Project Settings.


#include "AlzadiHUD.h"

#include "MainMenuWidget.h"
#include "SelectServerWidget.h"
#include "AlzadiWidget.h"
#include "Kismet/GameplayStatics.h"

AAlzadiHUD::AAlzadiHUD()
{
}

void AAlzadiHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!SelectServerWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SelectServerWidgetClass in %s."), *GetName());
		return;
	}

	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidgetClass in %s."), *GetName());
		return;
	}

	if (!AlzadiWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiWidgetClass in %s."), *GetName());
		return;
	}

	if (GetOwningPlayerController()->IsLocalController())
	{
		SelectServerWidget = CreateWidget<USelectServerWidget>(
			GetWorld()->GetFirstPlayerController(), SelectServerWidgetClass);
		SelectServerWidget->AddToViewport();
		SelectServerWidget->SetVisibility(ESlateVisibility::Visible);
	}


	MainMenuWidget = CreateWidget<UMainMenuWidget>(
		GetWorld()->GetFirstPlayerController(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);


	AlzadiWidget = CreateWidget<UAlzadiWidget>(
		GetWorld()->GetFirstPlayerController(), AlzadiWidgetClass);
	AlzadiWidget->AddToViewport();
	AlzadiWidget->SetVisibility(ESlateVisibility::Hidden);
}
