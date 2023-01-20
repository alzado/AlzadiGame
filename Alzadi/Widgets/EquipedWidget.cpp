// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipedWidget.h"

#include "ItemSlotIcon.h"
#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Inventory/EquipmentComponent.h"
#include "Alzadi/Items/ItemBase.h"


UEquipedWidget::UEquipedWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEquipedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	//
	// if (BaseCharacterReference)
	// {
	// 	BaseCharacterReference->EquipmentComponent->OnEquipmentComponentUpdate.AddUObject(
	// 		this, &UEquipedWidget::OnEquipmentComponentUpdate);
	// }
}

void UEquipedWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	BaseCharacterReference->EquipmentComponent->OnEquipedEvent.AddUObject(
		this, &UEquipedWidget::OnEquipmentComponentEquipedUpdate);
}

// void UEquipedWidget::SetBaseCharacterReference(ABaseCharacter* Character)
// {
// 	BaseCharacterReference = Character;
// }

void UEquipedWidget::OnEquipmentComponentEquipedUpdate()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	if (!HelmetHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HelmetHUD in %s."), *GetName());
		return;
	}

	if (!WeaponHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing WeaponHUD in %s."), *GetName());
		return;
	}

	if (!ArmorHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ArmorHUD in %s."), *GetName());
		return;
	}

	if (!ShieldHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ShieldHUD in %s."), *GetName());
		return;
	}

	if (!LegHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing LegHUD in %s."), *GetName());
		return;
	}

	if (!BootHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BootHUD in %s."), *GetName());
		return;
	}
	
	// Set fixed items

	HelmetHUD->SetItemBaseReference(BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Helmet]);
	WeaponHUD->SetItemBaseReference(BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Weapon]);
	ArmorHUD->SetItemBaseReference(BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Armor]);
	ShieldHUD->SetItemBaseReference(BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Shield]);
	LegHUD->SetItemBaseReference(BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Legs]);
	BootHUD->SetItemBaseReference(BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Boots]);
}
