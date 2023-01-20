// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class ABaseCharacter;
class UItemBase;

DECLARE_MULTICAST_DELEGATE(FOnEquipedEvent);
DECLARE_MULTICAST_DELEGATE(FOnInventoryEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALZADI_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentComponent();

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Set owner reference
	TObjectPtr<ABaseCharacter> BaseCharacter;

	void SetBaseCharacter(ABaseCharacter* Character);

	// reset component
	// void ResetComponent();

	// Equiped items
	UPROPERTY(ReplicatedUsing=OnRep_Equiped, EditDefaultsOnly, Category="Equiped")
	TArray<UItemBase*> Equiped;

	TArray<UItemBase*> GetEquiped();

	UFUNCTION()
	void OnRep_Equiped();

	void OnEquipedUpdate();

	FOnEquipedEvent OnEquipedEvent;

	// Web Server communication
	void SetEquipedFromData(TArray<FItem> InEquipedData);

	TArray<FItem> GetEquipedData();

	// Set equipment from item (used for drag and drop operations)
	UFUNCTION(Server, Reliable, WithValidation)
	void SetEquipedItemFromDragAndDrop(UItemBase* NewItem, UItemBase* OriginalItem);

	UPROPERTY(Replicated)
	bool bDragAndDropEnabled = true;

	// Inventory Items
	UPROPERTY(ReplicatedUsing=OnRep_Inventory, EditDefaultsOnly, Category="Equiped")
	TArray<UItemBase*> Inventory;

	UFUNCTION()
	void OnRep_Inventory();

	void OnInventoryUpdate();

	FOnInventoryEvent OnInventoryEvent;

	// Web Server communication
	void SetInventoryFromData(TArray<FItem> InInventoryData);

	TArray<FItem> GetInventoryData();
};
