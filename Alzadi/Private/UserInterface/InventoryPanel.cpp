// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/InventoryPanel.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Inventory/EquipmentComponent.h"
#include "Alzadi/Items/ItemBase.h"
#include "Alzadi/Widgets/ItemSlotIcon.h"
#include "Components/WrapBox.h"
#include "UserInterface/ItemDragDropOperation.h"


UInventoryPanel::UInventoryPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	BaseCharacterReference->EquipmentComponent->OnInventoryEvent.AddUObject(
		this, &UInventoryPanel::OnEquipmentComponentInventoryUpdate);
}

// void UInventoryPanel::SetBaseCharacterReference(ABaseCharacter* Character)
// {
// 	BaseCharacterReference = Character;
// }

void UInventoryPanel::OnEquipmentComponentInventoryUpdate()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponentReference in %s."), *GetName());
		return;
	}

	if (!ItemSlotIconClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemSlotIconClass in %s."), *GetName());
		return;
	}

	if (!InventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing InventoryPanel in %s."), *GetName());
		return;
	}

	InventoryPanel->ClearChildren();

	for (UItemBase* const& InventoryItem : BaseCharacterReference->EquipmentComponent->Inventory)
	{
		UItemSlotIcon* ItemSlotIcon = CreateWidget<UItemSlotIcon>(this, ItemSlotIconClass);

		ItemSlotIcon->SetItemBaseReference(InventoryItem);
		InventoryPanel->AddChildToWrapBox(ItemSlotIcon);
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                   UDragDropOperation* InOperation)
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return false;
	}

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *BaseCharacterReference->GetName());
		return false;
	}

	UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);
	UItemBase* DropedItem = Cast<UItemBase>(ItemDragDropOperation->Payload);

	BaseCharacterReference->EquipmentComponent->
	                        SetEquipedItemFromDragAndDrop(DropedItem, DropedItem);
	return true;
}
