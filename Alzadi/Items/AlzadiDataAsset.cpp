// Fill out your copyright notice in the Description page of Project Settings.


#include "AlzadiDataAsset.h"

// TPair<FName, FName> UAlzadiDataAsset::GetTypeAndSubType(FName Id)
// {
// 	TPair<FName, FName> Pair;
//
// 	for (FItemType& Type : ItemDataAsset)
// 	{
// 		for (FItemSubType& SubType : Type.SubType)
// 		{
// 			if (SubType.ItemData.Contains(Id))
// 			{
// 				Pair.Key = Type.Type;
// 				Pair.Value = SubType.SubType;
//
// 				return Pair;
// 			}
// 		}
// 	}
//
// 	Pair.Key = FName(TEXT("None"));
// 	Pair.Value = FName(TEXT("None"));
//
// 	return Pair;
// }
//
// FItem UAlzadiDataAsset::GetItemById(FName Id)
// {
// 	FItem Item;
//
// 	for (FItemType& Type : ItemDataAsset)
// 	{
// 		for (FItemSubType& SubType : Type.SubType)
// 		{
// 			if (SubType.ItemData.Contains(Id))
// 			{
// 				Item = SubType.ItemData[Id];
// 				return Item;
// 			}
// 		}
// 	}
//
// 	return Item;
// }

UAlzadiDataAsset::UAlzadiDataAsset()
{
	NewPlayerGameInfo.CharacterType = ECharacterType::Player;
}

TArray<FName> UAlzadiDataAsset::GetAllItemOptions()
{
	TArray<FName> Options;

	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ItemDataTable in %s."), *GetName());
		return Options;
	}

	Options = ItemDataTable->GetRowNames();

	return Options;
}
