// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipedWidget.generated.h"

class ABaseCharacter;
class UItemSlotIcon;

UCLASS()
class ALZADI_API UEquipedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEquipedWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	// Set character reference
	TObjectPtr<ABaseCharacter> BaseCharacterReference;

	// void SetBaseCharacterReference(ABaseCharacter* Character);

	// Helmet Slot
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UItemSlotIcon* HelmetHUD;

	// Weapon Slot
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UItemSlotIcon* WeaponHUD;

	// Armor Slot
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UItemSlotIcon* ArmorHUD;

	// Shield Slot
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UItemSlotIcon* ShieldHUD;

	// Leg Slot
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UItemSlotIcon* LegHUD;

	// Boot Slot
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UItemSlotIcon* BootHUD;

	// On components update

	void OnEquipmentComponentEquipedUpdate();
};
