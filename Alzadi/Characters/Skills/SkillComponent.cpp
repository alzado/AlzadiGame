// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USkillComponent, Skill);
}

void USkillComponent::SetBaseCharacter(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}
	
	BaseCharacter = Character;
}

void USkillComponent::ResetComponent()
{
	FSkill ResetSkill;
	SetSkill(ResetSkill);
}

void USkillComponent::OnRep_Skill()
{
	OnSkillUpdate();
}

void USkillComponent::OnSkillUpdate()
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

	OnSkillEvent.Broadcast();
}

void USkillComponent::SetSkill(FSkill InSkill)
{
	Skill = InSkill;

	OnSkillUpdate();
}

FSkill USkillComponent::GetSkill()
{
	return Skill;
}
