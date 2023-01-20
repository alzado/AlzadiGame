// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

class UEquipmentComponent;
class UInventoryComponent;

UCLASS()
class ALZADI_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override;

	// all items

	UPROPERTY(Replicated)
	FItemInfo ItemInfo;

	UPROPERTY(Replicated)
	FName Id;

	UPROPERTY(Replicated)
	int32 EquipedPosition = 0;

	UPROPERTY(Replicated)
	bool bIsEquiped = false;

	UPROPERTY(Replicated)
	float Quantity = 0.f;

	UPROPERTY(EditDefaultsOnly, Category=Items)
	float StackLimit = 100.f;

	void SetItemFromData(FItemInfo InItemInfo);
};
