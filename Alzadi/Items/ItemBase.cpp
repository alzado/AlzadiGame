// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "Net/UnrealNetwork.h"


void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	// set from character information

	DOREPLIFETIME(UItemBase, Id);
	DOREPLIFETIME(UItemBase, EquipedPosition);
	DOREPLIFETIME(UItemBase, bIsEquiped);
	DOREPLIFETIME(UItemBase, Quantity);

	// set from data table with Id
	DOREPLIFETIME(UItemBase, ItemInfo);
}

bool UItemBase::IsSupportedForNetworking() const
{
	return true;
}

void UItemBase::SetItemFromData(FItemInfo InItemInfo)
{
	ItemInfo = InItemInfo;
}
