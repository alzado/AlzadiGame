// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Health/HealthComponent.h"
#include "Alzadi/Core/AlzadiGameMode.h"
#include "Alzadi/Widgets/CharacterInfoWidget.h"
#include "Components/AlzadiProxyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

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
}

void UExperienceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UExperienceComponent, Experience);
	DOREPLIFETIME(UExperienceComponent, KillExperience);
	DOREPLIFETIME(UExperienceComponent, Level);
	DOREPLIFETIME(UExperienceComponent, ExpLevelDown);
	DOREPLIFETIME(UExperienceComponent, ExpLevelUp);
}

void UExperienceComponent::SetBaseCharacter(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	BaseCharacter = Character;
}

void UExperienceComponent::ResetExperienceComponent()
{
	SetExperience(0.f);
}

float UExperienceComponent::GetExpLevelUp()
{
	return ExpLevelUp;
}

// Experience functions

float UExperienceComponent::GetExpLevelDown()
{
	return ExpLevelDown;
}

void UExperienceComponent::SetExpLevelUpAndDown()
{
	// // loop through exp data base
	// for (FName& RowName : BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ExperienceDataTable->GetRowNames())
	// {
	// 	// get row info
	// 	FExpLevel RowInfo = *BaseCharacter->AlzadiGameModeRef->AlzadiDataAsset->ExperienceDataTable->FindRow<
	// 		FExpLevel>(RowName,TEXT("lol"));
	//
	// 	// test if its current exp is lower than level exp
	// 	if (Experience < RowInfo.Experience)
	// 	{
	// 		ExpLevelUp = RowInfo.Experience;
	// 		SetLevel(RowInfo.Level - 1);
	// 		break;
	// 	}
	//
	// 	// always stores the down exp one level before 
	// 	ExpLevelDown = RowInfo.Experience;
	// }

	// loop through levels
	for (int32 Index = 0; Index < 10000; ++Index)
	{
		ExpLevelUp = BaseCharacter->AlzadiGameModeRef->FormulaLevelExperience((float)Index);

		if (Experience < ExpLevelUp)
		{
			SetLevel((float)(Index - 1));
			break;
		}

		ExpLevelDown = BaseCharacter->AlzadiGameModeRef->FormulaLevelExperience((float)Index);
	}
}

void UExperienceComponent::OnRep_Experience()
{
	OnExperienceUpdate();
}

void UExperienceComponent::OnExperienceUpdate()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	// First try in server
	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		// test if limits of exp where breached
		if (Experience < ExpLevelDown || Experience >= ExpLevelUp)
		{
			SetExpLevelUpAndDown();
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
		// Get local client controller
		if (BaseCharacter->IsLocallyControlled())
		{
		}
		else
		{
		}
	}

	// Let others know it was update
	OnExperienceEvent.Broadcast();
}


void UExperienceComponent::SetExperience(float InExperience)
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		Experience = InExperience;

		OnExperienceUpdate();

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

float UExperienceComponent::GetExperience()
{
	return Experience;
}

// Level functions

void UExperienceComponent::OnRep_Level()
{
	OnLevelUpdate();
}

void UExperienceComponent::OnLevelUpdate()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}
	
	if (!BaseCharacter->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	BaseCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseCharacter->AlzadiGameModeRef->
	                                                                     FormulaSpeedLevel(Level, 0.f);

	if (BaseCharacter->IsPlayerControlled())
	{
		if (!BaseCharacter->AlzadiProxyCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiProxyCharacter in %s."), *GetName());
			return;
		}

		BaseCharacter->AlzadiProxyCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseCharacter->
			GetCharacterMovement()->
			MaxWalkSpeed;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		BaseCharacter->HealthComponent->SetMaxHealth(GetLevel(), 0.f);
		BaseCharacter->HealthComponent->SetHealth(BaseCharacter->HealthComponent->GetMaxHealth());


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

	OnLevelEvent.Broadcast();
}

void UExperienceComponent::SetLevel(float InLevel)
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->AlzadiGameModeRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiGameModeRef in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		Level = InLevel;

		OnLevelUpdate();

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

float UExperienceComponent::GetLevel()
{
	return Level;
}

void UExperienceComponent::SetKillExperience(float InKillExperience)
{
	KillExperience = InKillExperience;
}

float UExperienceComponent::GetKillExperience()
{
	return KillExperience;
}

void UExperienceComponent::GiveExperienceWhenKilled()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacter->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	// get total damage dealt by attackers
	float TotalDamage = 0.f;

	for (TPair<ABaseCharacter*, float>& SingleAttackerDamage : BaseCharacter->HealthComponent->GetAttackerDamage())
	{
		TotalDamage += SingleAttackerDamage.Value;
	}

	// add experience based on each attacker damage
	for (TPair<ABaseCharacter*, float>& SingleAttackerDamage : BaseCharacter->HealthComponent->GetAttackerDamage())
	{
		if (SingleAttackerDamage.Key)
		{
			if (SingleAttackerDamage.Key->ExperienceComponent)
			{
				SingleAttackerDamage.Key->ExperienceComponent->SetExperience(
					SingleAttackerDamage.Key->ExperienceComponent->GetExperience() + KillExperience *
					SingleAttackerDamage.
					Value
					/ TotalDamage);
			}
		}
	}
}
