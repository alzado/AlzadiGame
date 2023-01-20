// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInfoWidget.h"

#include "HealthBarWidget.h"
#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Experience/ExperienceComponent.h"
#include "Alzadi/Characters/Health/HealthComponent.h"
#include "Alzadi/Characters/Inventory/EquipmentComponent.h"
#include "Alzadi/Characters/Skills/SkillComponent.h"
#include "Alzadi/Items/ItemBase.h"
#include "Components/TextBlock.h"
#include "Interfaces/ExperienceBarWidget.h"


UCharacterInfoWidget::UCharacterInfoWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UCharacterInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	//
	// if (BaseCharacterReference)
	// {
	// 	// Create health bar
	// 	HealthBarWidget = CreateWidget<UHealthBarWidget>(this, HealthBarClass);
	// 	HealthBarWidget->SetBaseCharacterReference(BaseCharacterReference);
	// 	HealthBarWidget->SetupWidget();
	// 	HealthBarSizeBox->AddChild(HealthBarWidget);
	//
	// 	BaseCharacterReference->ExperienceComponent->OnExperienceComponentUpdate.AddUObject(
	// 		this, &UCharacterInfoWidget::OnExperienceComponentUpdate);
	// 	BaseCharacterReference->HealthComponent->OnHealthComponentUpdate.AddUObject(
	// 		this, &UCharacterInfoWidget::OnHealthComponentUpdate);
	// 	BaseCharacterReference->EquipmentComponent->OnEquipmentComponentUpdate.AddUObject(
	// 		this, &UCharacterInfoWidget::OnEquipmentComponentUpdate);
	// 	BaseCharacterReference->SkillComponent->OnSkillComponentUpdate.AddUObject(
	// 		this, &UCharacterInfoWidget::OnSkillComponentUpdate);
	// }
}

void UCharacterInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *GetName());
		return;
	}

	if (!HealthBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthBarWidget in %s."), *GetName());
		return;
	}

	ExperienceBarWidget->SetBaseCharacterRef(BaseCharacterReference);
	ExperienceBarWidget->SetupWidget();

	HealthBarWidget->SetBaseCharacterReference(BaseCharacterReference);
	HealthBarWidget->SetupWidget();


	BaseCharacterReference->ExperienceComponent->OnExperienceEvent.AddUObject(
		this, &UCharacterInfoWidget::OnExperienceComponentExperienceUpdate);
	BaseCharacterReference->ExperienceComponent->OnLevelEvent.AddUObject(
		this, &UCharacterInfoWidget::OnExperienceComponentLevelUpdate);

	BaseCharacterReference->HealthComponent->OnHealthEvent.AddUObject(
		this, &UCharacterInfoWidget::OnHealthComponentHealthUpdate);
	BaseCharacterReference->HealthComponent->OnMaxHealthEvent.AddUObject(
		this, &UCharacterInfoWidget::OnHealthComponentMaxHealthUpdate);

	BaseCharacterReference->EquipmentComponent->OnEquipedEvent.AddUObject(
		this, &UCharacterInfoWidget::OnEquipmentComponentEquipedUpdate);

	BaseCharacterReference->SkillComponent->OnSkillEvent.AddUObject(
		this, &UCharacterInfoWidget::OnSkillComponentSkillUpdate);
}

// void UCharacterInfoWidget::SetBaseCharacterReference(ABaseCharacter* Character)
// {
// 	BaseCharacterReference = Character;
// }

void UCharacterInfoWidget::SetExperienceHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!ExperienceHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceHUD in %s."), *GetName());
		return;
	}

	ExperienceHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->ExperienceComponent->GetExperience())));
}

void UCharacterInfoWidget::SetLevelHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!LevelHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing LevelHUD in %s."), *GetName());
		return;
	}

	LevelHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->ExperienceComponent->GetLevel())));
}

void UCharacterInfoWidget::SetHealthHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	if (!HealthHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthHUD in %s."), *GetName());
		return;
	}

	HealthHUD->SetText(FText::FromString(FString::SanitizeFloat(BaseCharacterReference->HealthComponent->GetHealth())));
}

void UCharacterInfoWidget::SetMaxHealthHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	if (!MaxHealthHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MaxHealthHUD in %s."), *GetName());
		return;
	}

	MaxHealthHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->HealthComponent->GetMaxHealth())));
}

void UCharacterInfoWidget::SetMeleeSkillHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *GetName());
		return;
	}

	if (!MeleeSkillHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MeleeSkillHUD in %s."), *GetName());
		return;
	}

	MeleeSkillHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->SkillComponent->GetSkill().Melee)));
}

void UCharacterInfoWidget::SetDistanceSkillHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	if (!DistanceSkillHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing DistanceSkillHUD in %s."), *GetName());
		return;
	}

	DistanceSkillHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->SkillComponent->GetSkill().Distance)));
}

void UCharacterInfoWidget::SetMagicSkillHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	if (!MagicSkillHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MagicSkillHUD in %s."), *GetName());
		return;
	}

	MagicSkillHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->SkillComponent->GetSkill().Magic)));
}

void UCharacterInfoWidget::SetDefenseSkillHUD()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	if (!DefenseSkillHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing DefenseSkillHUD in %s."), *GetName());
		return;
	}

	DefenseSkillHUD->SetText(
		FText::FromString(FString::SanitizeFloat(BaseCharacterReference->SkillComponent->GetSkill().Defense)));
}

void UCharacterInfoWidget::SetWeaponDamageHUD()
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

	if (!BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Weapon])
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Weapon in %s."), *GetName());
		return;
	}

	if (!WeaponDamageHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing WeaponDamageHUD in %s."), *GetName());
		return;
	}

	WeaponDamageHUD->SetText(
		FText::FromString(FString::SanitizeFloat(
			BaseCharacterReference->EquipmentComponent->Equiped[(int32)EEquipment::Weapon]->ItemInfo.Weapon.Damage)));
}

void UCharacterInfoWidget::OnExperienceComponentExperienceUpdate()
{
	SetExperienceHUD();
}

void UCharacterInfoWidget::OnExperienceComponentLevelUpdate()
{
	SetLevelHUD();
}

void UCharacterInfoWidget::OnHealthComponentHealthUpdate()
{
	SetHealthHUD();
}

void UCharacterInfoWidget::OnHealthComponentMaxHealthUpdate()
{
	SetMaxHealthHUD();
}

void UCharacterInfoWidget::OnEquipmentComponentEquipedUpdate()
{
	SetWeaponDamageHUD();
}

void UCharacterInfoWidget::OnSkillComponentSkillUpdate()
{
	SetMeleeSkillHUD();
	SetDistanceSkillHUD();
	SetMagicSkillHUD();
	SetDefenseSkillHUD();
}
