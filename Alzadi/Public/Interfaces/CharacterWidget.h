// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

class ABaseCharacter;
class UHealthBarWidget;
class UTextBlock;
class UWidgetAnimation;

UCLASS()
class ALZADI_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetupWidget();

	// set character reference
	TObjectPtr<ABaseCharacter> BaseCharacterRef;
	
	void SetBaseCharacterRef(ABaseCharacter* BaseCharacter);

	UPROPERTY(meta=(BindWidget))
	UHealthBarWidget* HealthBarWidget;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* HitPoints;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* HitPointsAnimation;

	// on components update
	void OnHealthComponentHealthUpdate();

	FTimerHandle TimerHandle;
	void StopTimer();
};
