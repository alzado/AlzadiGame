// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "AlzadiDataAsset.generated.h"


UCLASS(BlueprintType)
class ALZADI_API UAlzadiDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UAlzadiDataAsset();

public:
	// items

	UPROPERTY(EditDefaultsOnly, Category=Items, meta=(RequiredAssetDataTags="RowStructure=ItemInfo"))
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category=Items, meta=(GetOptions="GetAllItemOptions"))
	FName BlankItemId;

	UFUNCTION()
	TArray<FName> GetAllItemOptions();

	// player

	UPROPERTY(EditDefaultsOnly, Category=Players)
	FAlzadiCharacter NewPlayerGameInfo;


	// monsters

	UPROPERTY(EditDefaultsOnly, Category=Monsters, meta=(RequiredAssetDataTags="RowStructure=AlzadiCharacter"))
	UDataTable* MonsterDataTable;
};
