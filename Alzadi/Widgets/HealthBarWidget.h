// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class ABaseCharacter;
class UProgressBar;
class UTextBlock;

UCLASS()
class ALZADI_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UHealthBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetupWidget();

	// Set character reference
	TObjectPtr<ABaseCharacter> BaseCharacterReference;

	void SetBaseCharacterReference(ABaseCharacter* Character);

	// Percentage
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBarHUD;

	void SetHealthBar();

	// On components update
	void OnHealthComponentHealthUpdate();
	void OnHealthComponentMaxHealthUpdate();
};
