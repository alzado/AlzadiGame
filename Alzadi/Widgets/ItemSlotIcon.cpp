// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotIcon.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Inventory/EquipmentComponent.h"
#include "Alzadi/Items/ItemBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UserInterface/DragItemVisual.h"
#include "UserInterface/ItemDragDropOperation.h"


UItemSlotIcon::UItemSlotIcon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UItemSlotIcon::NativeConstruct()
{
	Super::NativeConstruct();

	// // casts connected character here because there could be much of these
	// if (Cast<ABaseCharacter>(GetOwningPlayerPawn()))
	// {
	// 	BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	// }
}

void UItemSlotIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// casts connected character here because there could be much of these
	BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());
}

void UItemSlotIcon::SetItemBaseReference(UItemBase* Item)
{
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Item in %s."), *GetName());
		return;
	}

	if (!IconHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing IconHUD in %s."), *GetName());
		return;
	}

	if (!Quantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Quantity in %s."), *GetName());
		return;
	}

	ItemBaseReference = Item;

	IconHUD->SetBrushFromTexture(ItemBaseReference->ItemInfo.Icon);
	Quantity->SetText(FText::FromString(FString::SanitizeFloat(ItemBaseReference->Quantity)));
}

FReply UItemSlotIcon::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	return Reply;
}

void UItemSlotIcon::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                         UDragDropOperation*& OutOperation)
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *GetName());
		return;
	}

	if (!DragItemVisualClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing DragItemVisualClass in %s."), *GetName());
		return;
	}

	if (!ItemBaseReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemBaseReference in %s."), *GetName());
		return;
	}

	if (ItemBaseReference->Id != TEXT("Blank"))
	{
		if (BaseCharacterReference->EquipmentComponent->bDragAndDropEnabled)
		{
			UDragItemVisual* DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
			DragItemVisual->ItemIcon->SetBrushFromTexture(ItemBaseReference->ItemInfo.Icon);
			DragItemVisual->Quantity->SetText(
				FText::FromString(FString::SanitizeFloat(ItemBaseReference->Quantity)));

			UItemDragDropOperation* ItemDragDropOperation = NewObject<UItemDragDropOperation>();
			ItemDragDropOperation->Payload = ItemBaseReference;
			ItemDragDropOperation->DefaultDragVisual = DragItemVisual;
			ItemDragDropOperation->Pivot = EDragPivot::TopLeft;

			OutOperation = ItemDragDropOperation;
		}
	}
}

bool UItemSlotIcon::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation)
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return false;
	}

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *GetName());
		return false;
	}

	if (!ItemBaseReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemBaseReference in %s."), *GetName());
		return false;
	}

	UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);
	UItemBase* DropedItem = Cast<UItemBase>(ItemDragDropOperation->Payload);

	BaseCharacterReference->EquipmentComponent->
	                        SetEquipedItemFromDragAndDrop(DropedItem, ItemBaseReference);
	return true;
}
