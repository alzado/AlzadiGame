// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class ABaseCharacter;
class UItemSlotIcon;
class UWrapBox;

UCLASS()
class ALZADI_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryPanel(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// Set player reference
	TObjectPtr<ABaseCharacter> BaseCharacterReference;

	// void SetBaseCharacterReference(ABaseCharacter* Character);

	// Set parameters
	UPROPERTY(EditDefaultsOnly, Category="Items")
	TSubclassOf<UItemSlotIcon> ItemSlotIconClass;

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;

	// Set updates

	UFUNCTION()
	void OnEquipmentComponentInventoryUpdate();

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;
};
