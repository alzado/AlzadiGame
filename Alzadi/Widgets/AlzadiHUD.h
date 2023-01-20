// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AlzadiHUD.generated.h"

class USelectServerWidget;
class UMainMenuWidget;
class UAlzadiWidget;

UCLASS()
class ALZADI_API AAlzadiHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAlzadiHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<USelectServerWidget> SelectServerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UAlzadiWidget> AlzadiWidgetClass;

	UPROPERTY()
	USelectServerWidget* SelectServerWidget;

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	UAlzadiWidget* AlzadiWidget;
};
