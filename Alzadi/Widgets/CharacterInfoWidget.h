// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInfoWidget.generated.h"

class ABaseCharacter;
class UTextBlock;
class UHealthBarWidget;
class UExperienceBarWidget;
class UExperienceComponent;

UCLASS()
class ALZADI_API UCharacterInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCharacterInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	// Set character reference
	// TObjectPtr<ABaseCharacter> BaseCharacterReference;
	ABaseCharacter* BaseCharacterReference;

	// void SetBaseCharacterReference(ABaseCharacter* Character);

	// Current state variables
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ExperienceHUD;

	void SetExperienceHUD();

	UPROPERTY(meta=(BindWidget))
	UExperienceBarWidget* ExperienceBarWidget;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelHUD;

	void SetLevelHUD();

	// UPROPERTY(meta=(BindWidget))
	// USizeBox* HealthBarSizeBox;

	UPROPERTY(meta=(BindWidget))
	UHealthBarWidget* HealthBarWidget;

	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<UHealthBarWidget> HealthBarClass;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthHUD;

	void SetHealthHUD();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHealthHUD;

	void SetMaxHealthHUD();

	// skills
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MeleeSkillHUD;

	void SetMeleeSkillHUD();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DistanceSkillHUD;

	void SetDistanceSkillHUD();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MagicSkillHUD;

	void SetMagicSkillHUD();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DefenseSkillHUD;

	void SetDefenseSkillHUD();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeaponDamageHUD;

	void SetWeaponDamageHUD();

	// On components update
	void OnExperienceComponentExperienceUpdate();
	void OnExperienceComponentLevelUpdate();

	void OnHealthComponentHealthUpdate();
	void OnHealthComponentMaxHealthUpdate();

	void OnEquipmentComponentEquipedUpdate();

	void OnSkillComponentSkillUpdate();
};
