// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExperienceBarWidget.generated.h"

class ABaseCharacter;
class UProgressBar;

UCLASS()
class ALZADI_API UExperienceBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetupWidget();

	// character reference
	TObjectPtr<ABaseCharacter> BaseCharacterRef;
	
	void SetBaseCharacterRef(ABaseCharacter* InBaseCharacter);

	// Percentage
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ExperienceBarHUD;

	void SetExperienceBarHUD();

	// On components update
	void OnExperienceComponentExperienceUpdate();
	void OnExperienceComponentLevelUpdate();
};
