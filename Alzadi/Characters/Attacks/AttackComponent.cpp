// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Experience/ExperienceComponent.h"
#include "Alzadi/Characters/Inventory/EquipmentComponent.h"
#include "Alzadi/Characters/Skills/SkillComponent.h"
#include "Alzadi/Core/AlzadiGameMode.h"
#include "Alzadi/Items/ItemBase.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		// binders
		BaseCharacter->EquipmentComponent->OnEquipedEvent.AddUObject(
			this, &UAttackComponent::OnEquipmentComponentEquipedUpdate);

		BaseCharacter->ExperienceComponent->OnLevelEvent.AddUObject(
			this, &UAttackComponent::OnExperienceComponentLevelUpdate);

		BaseCharacter->SkillComponent->OnSkillEvent.AddUObject(
			this, &UAttackComponent::OnSkillComponentSkillUpdate);

		// timer
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAttackComponent::BaseAttackLoop);
		BaseCharacter->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, BaseAttackRate, true);

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
}

void UAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttackComponent, BaseAttack);
	DOREPLIFETIME(UAttackComponent, DamageMultiplier);
}

void UAttackComponent::SetBaseCharacter(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	BaseCharacter = Character;
}

void UAttackComponent::SetDamageMultiplier()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiGameModeRef in %s."),
		       *GetName());
		return;
	}

	if (!BaseCharacter->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *GetName());
		return;
	}

	DamageMultiplier.Level = BaseCharacter->AlzadiGameModeRef->FormulaLevelDamage(
		BaseCharacter->ExperienceComponent->GetLevel());
	DamageMultiplier.Melee = BaseCharacter->AlzadiGameModeRef->FormulaSkillDamage(
		BaseCharacter->SkillComponent->GetSkill().Melee);
	DamageMultiplier.Distance = BaseCharacter->AlzadiGameModeRef->FormulaSkillDamage(
		BaseCharacter->SkillComponent->GetSkill().Distance);
	DamageMultiplier.Magic = BaseCharacter->AlzadiGameModeRef->FormulaSkillDamage(
		BaseCharacter->SkillComponent->GetSkill().Magic);
}

FDamageMultiplier UAttackComponent::GetDamageMultiplier()
{
	return DamageMultiplier;
}

void UAttackComponent::BaseAttackLoop()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	// if any locked target
	if (!BaseCharacter->LockedTarget)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Missing LockedTarget in %s."), *BaseCharacter->GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		// see if target is in range
		if (FVector::Distance(BaseCharacter->GetActorLocation(), BaseCharacter->LockedTarget->GetActorLocation()) <=
			BaseAttack.Range)
		{
			// see if weapon is melee
			if (BaseAttack.Type == EAttackType::Melee)
			{
				UGameplayStatics::ApplyDamage(Cast<AActor>(BaseCharacter->LockedTarget),
				                              BaseAttack.Damage + DamageMultiplier.Level + DamageMultiplier.Melee,
				                              BaseCharacter->GetController(), Cast<AActor>(BaseCharacter),
				                              UDamageType::StaticClass()
				);
			}

			// see if weapon is distance
			if (BaseAttack.Type == EAttackType::Distance)
			{
				UGameplayStatics::ApplyDamage(Cast<AActor>(BaseCharacter->LockedTarget),
				                              BaseAttack.Damage + DamageMultiplier.Level + DamageMultiplier.Distance,
				                              BaseCharacter->GetController(), Cast<AActor>(BaseCharacter),
				                              UDamageType::StaticClass());
			}

			// see if weapon is magic
			if (BaseAttack.Type == EAttackType::Magic)
			{
				UGameplayStatics::ApplyDamage(Cast<AActor>(BaseCharacter->LockedTarget),
				                              BaseAttack.Damage + DamageMultiplier.Level + DamageMultiplier.Magic,
				                              BaseCharacter->GetController(), Cast<AActor>(BaseCharacter),
				                              UDamageType::StaticClass());
			}
		}

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
}

void UAttackComponent::SetBaseAttack(FAttack Attack)
{
	BaseAttack = Attack;
}

FAttack UAttackComponent::GetBaseAttack()
{
	return BaseAttack;
}

void UAttackComponent::OnRep_BaseAttack()
{
	OnBaseAttackUpdate();
}

void UAttackComponent::OnBaseAttackUpdate()
{
}

void UAttackComponent::OnEquipmentComponentEquipedUpdate()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *BaseCharacter->GetName());
		return;
	}

	if (!BaseCharacter->EquipmentComponent->GetEquiped()[(int32)EEquipment::Weapon])
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Weapon in %s."), *GetName());
		return;
	}

	SetBaseAttack(BaseCharacter->EquipmentComponent->GetEquiped()[(int32)EEquipment::Weapon]->ItemInfo.Weapon);

	OnBaseAttackUpdate();
}

void UAttackComponent::OnExperienceComponentLevelUpdate()
{
	SetDamageMultiplier();
}

void UAttackComponent::OnSkillComponentSkillUpdate()
{
	SetDamageMultiplier();
}
