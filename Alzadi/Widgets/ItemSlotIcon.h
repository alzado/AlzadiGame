// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotIcon.generated.h"

class ABaseCharacter;
class UImage;
class UDragItemVisual;
class UItemBase;
class UTextBlock;

UCLASS()
class ALZADI_API UItemSlotIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemSlotIcon(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	// Character reference
	TObjectPtr<ABaseCharacter> BaseCharacterReference;

	// Item reference
	TObjectPtr<UItemBase> ItemBaseReference;
	void SetItemBaseReference(UItemBase* ItemBase);

	// Icon
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UImage* IconHUD;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Quantity;

	// Drag visual class
	UPROPERTY(EditDefaultsOnly, Category="Drag Visual")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;
};
