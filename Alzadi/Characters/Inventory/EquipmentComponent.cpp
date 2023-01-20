// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Core/AlzadiGameMode.h"
#include "Alzadi/Items/ItemBase.h"
#include "Engine/ActorChannel.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

bool UEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bWroteSomething |= Channel->ReplicateSubobjectList(Equiped, *Bunch, *RepFlags);
	bWroteSomething |= Channel->ReplicateSubobjectList(Inventory, *Bunch, *RepFlags);

	return bWroteSomething;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, bDragAndDropEnabled);
	DOREPLIFETIME(UEquipmentComponent, Equiped);
	DOREPLIFETIME(UEquipmentComponent, Inventory);
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
	}
}

void UEquipmentComponent::SetBaseCharacter(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	BaseCharacter = Character;
}

// void UEquipmentComponent::ResetComponent()
// {
// 	Equiped.ResetUstruct();
// 	Inventory.Empty();
//
// 	if (Equiped.Num() > 0)
// 	{
// 		OnEquipedUpdate();
// 	}
//
// 	if (Inventory.Num() > 0)
// 	{
// 		OnInventoryUpdate();
// 	}
// }

TArray<UItemBase*> UEquipmentComponent::GetEquiped()
{
	return Equiped;
}

void UEquipmentComponent::OnRep_Equiped()
{
	if (Equiped.Num() > 0)
	{
		OnEquipedUpdate();
	}
}

void UEquipmentComponent::OnEquipedUpdate()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		if (BaseCharacter->IsLocallyControlled())
		{
		}
		else
		{
		}
	}
	else
	{
		if (BaseCharacter->IsLocallyControlled())
		{
		}
		else
		{
		}
	}

	OnEquipedEvent.Broadcast();
}

void UEquipmentComponent::SetEquipedFromData(TArray<FItem> InEquipedData)
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiGameMode in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiDataAsset in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemDataTable in %s."), *GetName());
		return;
	}

	for (int32 Index = 0; Index < InEquipedData.Num(); ++Index)
	{
		Equiped.Add(NewObject<UItemBase>());
		Equiped[Index]->bIsEquiped = true;
		Equiped[Index]->EquipedPosition = Index;
		Equiped[Index]->Id = InEquipedData[Index].Id;
		Equiped[Index]->Quantity = InEquipedData[Index].Quantity;
		Equiped[Index]->SetItemFromData(
			*BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
				Equiped[Index]->Id, TEXT("lol")));
	}

	// set correct item type for empty values
	Equiped[(int32)EEquipment::Helmet]->ItemInfo.ItemType = EItemType::Helmet;
	Equiped[(int32)EEquipment::Weapon]->ItemInfo.ItemType = EItemType::Weapon;
	Equiped[(int32)EEquipment::Armor]->ItemInfo.ItemType = EItemType::Armor;
	Equiped[(int32)EEquipment::Shield]->ItemInfo.ItemType = EItemType::Shield;
	Equiped[(int32)EEquipment::Legs]->ItemInfo.ItemType = EItemType::Legs;
	Equiped[(int32)EEquipment::Boots]->ItemInfo.ItemType = EItemType::Boots;

	OnEquipedUpdate();
}

TArray<FItem> UEquipmentComponent::GetEquipedData()
{
	TArray<FItem> OutEquipedData;

	for (int Index = 0; Index < Equiped.Num(); ++Index)
	{
		FItem SingleItemData;
		SingleItemData.Id = Equiped[Index]->Id;
		SingleItemData.Quantity = Equiped[Index]->Quantity;
		OutEquipedData.Add(SingleItemData);
	}
	return OutEquipedData;
}

void UEquipmentComponent::OnRep_Inventory()
{
	if (Inventory.Num() > 0)
	{
		OnInventoryUpdate();
	}
}

void UEquipmentComponent::OnInventoryUpdate()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		if (BaseCharacter->IsLocallyControlled())
		{
		}
		else
		{
		}
	}
	else
	{
		if (BaseCharacter->IsLocallyControlled())
		{
		}
		else
		{
		}
	}

	OnInventoryEvent.Broadcast();
}

void UEquipmentComponent::SetInventoryFromData(TArray<FItem> InInventoryData)
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiGameMode in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiDataAsset in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemDataTable in %s."), *GetName());
		return;
	}

	for (int32 Index = 0; Index < InInventoryData.Num(); ++Index)
	{
		Inventory.Add(NewObject<UItemBase>());
		Inventory[Index]->EquipedPosition = Index;
		Inventory[Index]->Id = InInventoryData[Index].Id;
		Inventory[Index]->Quantity = InInventoryData[Index].Quantity;
		Inventory[Index]->SetItemFromData(
			*BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
				Inventory[Index]->Id,TEXT("lol")));
	}

	OnInventoryUpdate();
}

TArray<FItem> UEquipmentComponent::GetInventoryData()
{
	TArray<FItem> OutInventoryData;

	for (int Index = 0; Index < Inventory.Num(); ++Index)
	{
		FItem SingleItemData;
		SingleItemData.Id = Inventory[Index]->Id;
		SingleItemData.Quantity = Inventory[Index]->Quantity;
		OutInventoryData.Add(SingleItemData);
	}

	return OutInventoryData;
}

void UEquipmentComponent::SetEquipedItemFromDragAndDrop_Implementation(UItemBase* NewItem, UItemBase* OriginalItem)
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiGameMode in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiDataAsset in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemDataTable in %s."), *GetName());
		return;
	}

	if (!NewItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing NewItem in %s."), *GetName());
		return;
	}

	if (!OriginalItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing OriginalItem in %s."), *GetName());
		return;
	}

	if (bDragAndDropEnabled)
	{
		bDragAndDropEnabled = false;

		int32 OriginalItemPosition = OriginalItem->EquipedPosition;
		int32 NewItemPosition = NewItem->EquipedPosition;
		float OriginalItemQuantity = OriginalItem->Quantity;
		float NewItemQuantity = NewItem->Quantity;
		float NewItemStackLimit = NewItem->StackLimit;

		// in case item moves from equipment to equipment (deberia salir pronto)
		if (NewItem->bIsEquiped && OriginalItem->bIsEquiped)
		{
			// in case is moved to inventory panel
			if (NewItem == OriginalItem)
			{
				// update equiped
				Equiped[NewItemPosition] = NewObject<UItemBase>();
				Equiped[NewItemPosition]->bIsEquiped = true;
				Equiped[NewItemPosition]->EquipedPosition = NewItemPosition;
				Equiped[NewItemPosition]->Id = BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->BlankItemId;
				Equiped[NewItemPosition]->Quantity = 1;
				Equiped[NewItemPosition]->SetItemFromData(
					*BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
						Equiped[NewItemPosition]->Id,TEXT("lol")));
				Equiped[NewItemPosition]->ItemInfo.ItemType = NewItem->ItemInfo.ItemType;

				// update inventory
				Inventory.Add(NewItem);
				NewItem->bIsEquiped = false;

				Inventory[Inventory.Num() - 1]->EquipedPosition = Inventory.Num() - 1;
			}
			else
			{
				// behavior for same id and stackable items
				if (NewItem->Id == OriginalItem->Id && NewItem->ItemInfo.bIsStackable)
				{
					// in case both items sum up more than limit
					if (NewItemQuantity + OriginalItemQuantity > NewItemStackLimit)
					{
						// swap both items to get replication, first change properties, then change item itself
						OriginalItem->Quantity = NewItemQuantity + OriginalItemQuantity -
							NewItemStackLimit;
						OriginalItem->EquipedPosition = NewItemPosition;

						NewItem->Quantity = NewItemStackLimit;
						NewItem->EquipedPosition = OriginalItemPosition;

						Equiped[NewItemPosition] = OriginalItem;
						Equiped[OriginalItemPosition] = NewItem;
					}
					// if no remanent, item should be replaced with empty space
					else
					{
						// add both quantities
						Equiped[OriginalItemPosition]->Quantity = NewItemQuantity + OriginalItemQuantity;

						// delete item
						Equiped[NewItemPosition] = NewObject<UItemBase>();
						Equiped[NewItemPosition]->bIsEquiped = true;
						Equiped[NewItemPosition]->EquipedPosition = NewItemPosition;
						Equiped[NewItemPosition]->Id = BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->BlankItemId;
						Equiped[NewItemPosition]->Quantity = 1;
						Equiped[NewItemPosition]->SetItemFromData(
							*BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
								Equiped[NewItemPosition]->Id,TEXT("lol")));
					}
				}
				else
				{
					if (NewItem->ItemInfo.ItemType == OriginalItem->ItemInfo.ItemType)
					{
						// update equiped
						Equiped[NewItem->EquipedPosition] = OriginalItem;
						Equiped[OriginalItem->EquipedPosition] = NewItem;

						NewItem->EquipedPosition = OriginalItemPosition;
						OriginalItem->EquipedPosition = NewItemPosition;
					}
				}
			}
		}
		// in case the new item comes from inventory to equipment
		else if (!NewItem->bIsEquiped && OriginalItem->bIsEquiped)
		{
			// behavior for same id and stackable items
			if (NewItem->Id == OriginalItem->Id && NewItem->ItemInfo.bIsStackable)
			{
				// in case both items sum up more than limit
				if (NewItemQuantity + OriginalItemQuantity > NewItemStackLimit)
				{
					// remanent stays at new item position
					OriginalItem->Quantity = NewItemQuantity + OriginalItemQuantity - NewItemStackLimit;
					OriginalItem->EquipedPosition = NewItemPosition;
					OriginalItem->bIsEquiped = false;

					// add maximum quantity to original item position
					NewItem->Quantity = NewItemStackLimit;
					NewItem->EquipedPosition = OriginalItemPosition;
					NewItem->bIsEquiped = true;

					Equiped[OriginalItemPosition] = NewItem;
					Inventory[NewItemPosition] = OriginalItem;
				}
				// if no remanent, item should be replaced with empty space
				else
				{
					// add both quantities
					Equiped[OriginalItemPosition]->Quantity = NewItemQuantity + OriginalItemQuantity;

					// delete item
					Inventory.RemoveAt(NewItemPosition);

					for (int32 Index = NewItemPosition; Index < Inventory.Num(); ++Index)
					{
						Inventory[Index]->EquipedPosition = Index;
					}
				}
			}
			else
			{
				if (NewItem->ItemInfo.ItemType == OriginalItem->ItemInfo.ItemType)
				{
					// update equipped
					Equiped[OriginalItemPosition] = NewItem;
					NewItem->EquipedPosition = OriginalItemPosition;
					NewItem->bIsEquiped = true;
					OriginalItem->bIsEquiped = false;

					// update inventory
					Inventory.RemoveAt(NewItemPosition);

					if (OriginalItem->Id != BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->BlankItemId)
					{
						Inventory.Add(OriginalItem);
					}

					for (int32 Index = NewItemPosition; Index < Inventory.Num(); ++Index)
					{
						Inventory[Index]->EquipedPosition = Index;
					}
				}
			}
		}
		// in case item comes from equipment to inventory
		else if (NewItem->bIsEquiped && !OriginalItem->bIsEquiped)
		{
			// behavior for same id and stackable items
			if (NewItem->Id == OriginalItem->Id && NewItem->ItemInfo.bIsStackable)
			{
				// in case both items sum up more than limit
				if (NewItemQuantity + OriginalItemQuantity > NewItemStackLimit)
				{
					// remanent stays at new item position
					OriginalItem->Quantity = NewItemQuantity + OriginalItemQuantity - NewItemStackLimit;
					OriginalItem->EquipedPosition = NewItemPosition;
					OriginalItem->bIsEquiped = false;

					// add maximum quantity to original item position
					NewItem->Quantity = NewItemStackLimit;
					NewItem->EquipedPosition = OriginalItemPosition;
					NewItem->bIsEquiped = false;

					Inventory[OriginalItemPosition] = NewItem;

					Inventory.Add(OriginalItem);
					Inventory[Inventory.Num() - 1]->EquipedPosition = Inventory.Num() - 1;

					// delete item
					Equiped[NewItemPosition] = NewObject<UItemBase>();
					Equiped[NewItemPosition]->bIsEquiped = true;
					Equiped[NewItemPosition]->EquipedPosition = NewItemPosition;
					Equiped[NewItemPosition]->Id = BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->BlankItemId;
					Equiped[NewItemPosition]->Quantity = 1;
					Equiped[NewItemPosition]->SetItemFromData(*
						BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
							Equiped[NewItemPosition]->Id,TEXT("lol")));
				}
				// if no remanent, item should be replaced with empty space
				else
				{
					// add both quantities
					Inventory[OriginalItemPosition]->Quantity = NewItemQuantity + OriginalItemQuantity;

					// delete item
					Equiped[NewItemPosition] = NewObject<UItemBase>();
					Equiped[NewItemPosition]->bIsEquiped = true;
					Equiped[NewItemPosition]->EquipedPosition = NewItemPosition;
					Equiped[NewItemPosition]->Id = BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->BlankItemId;
					Equiped[NewItemPosition]->Quantity = 1;
					Equiped[NewItemPosition]->SetItemFromData(*
						BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
							Equiped[NewItemPosition]->Id,TEXT("lol")));
				}
			}
			else
			{
				// update equiped
				Equiped[NewItemPosition] = NewObject<UItemBase>();
				Equiped[NewItemPosition]->bIsEquiped = true;
				Equiped[NewItemPosition]->EquipedPosition = NewItemPosition;
				Equiped[NewItemPosition]->Id = BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->BlankItemId;
				Equiped[NewItemPosition]->Quantity = 1;
				Equiped[NewItemPosition]->SetItemFromData(*
					BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ItemDataTable->FindRow<FItemInfo>(
						Equiped[NewItemPosition]->Id,TEXT("lol")));
				Equiped[NewItemPosition]->ItemInfo.ItemType = NewItem->ItemInfo.ItemType;

				// update inventory

				Inventory.Add(NewItem);
				NewItem->bIsEquiped = false;

				Inventory[Inventory.Num() - 1]->EquipedPosition = Inventory.Num() - 1;
			}
		}
		// in case item moves from inventory to inventory
		else if (!NewItem->bIsEquiped && !OriginalItem->bIsEquiped)
		{
			// in case is moved to inventory panel
			if (NewItem == OriginalItem)
			{
				// update inventory
				Inventory.RemoveAt(NewItemPosition);
				Inventory.Add(NewItem);

				for (int32 Index = NewItemPosition; Index < Inventory.Num(); ++Index)
				{
					Inventory[Index]->EquipedPosition = Index;
				}
			}
			else
			{
				// behavior for same id and stackable items
				if (NewItem->Id == OriginalItem->Id && NewItem->ItemInfo.bIsStackable)
				{
					// in case both items sum up more than limit
					if (NewItemQuantity + OriginalItemQuantity > NewItemStackLimit)
					{
						// remanent stays at new item position
						OriginalItem->Quantity = NewItemQuantity + OriginalItemQuantity - NewItemStackLimit;
						OriginalItem->EquipedPosition = NewItemPosition;

						// add maximum quantity to original item position
						NewItem->Quantity = NewItemStackLimit;
						NewItem->EquipedPosition = OriginalItemPosition;

						Inventory[OriginalItemPosition] = NewItem;
						Inventory[NewItemPosition] = OriginalItem;
					}
					// if no remanent, item should be replaced with empty space
					else
					{
						// add both quantities
						Inventory[OriginalItemPosition]->Quantity = NewItemQuantity + OriginalItemQuantity;

						// delete item
						Inventory.RemoveAt(NewItemPosition);

						for (int32 Index = NewItemPosition; Index < Inventory.Num(); ++Index)
						{
							Inventory[Index]->EquipedPosition = Index;
						}
					}
				}
				else
				{
					// update inventory
					Inventory.RemoveAt(NewItemPosition);
					Inventory.Add(NewItem);

					for (int32 Index = NewItemPosition; Index < Inventory.Num(); ++Index)
					{
						Inventory[Index]->EquipedPosition = Index;
					}
				}
			}
		}

		OnEquipedUpdate();
		OnInventoryUpdate();

		bDragAndDropEnabled = true;
	}
}

bool UEquipmentComponent::SetEquipedItemFromDragAndDrop_Validate(UItemBase* NewItem, UItemBase* OriginalItem)
{
	return true;
}
