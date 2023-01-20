// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Core/AlzadiGameMode.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, LastDamage);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		BaseCharacter->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

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

void UHealthComponent::SetBaseCharacter(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	BaseCharacter = Character;
}

void UHealthComponent::ResetComponent()
{
	SetHealth(1.f);
	SetMaxHealth(1.f, 0.f);
}

// Current health

void UHealthComponent::OnRep_Health()
{
	OnHealthUpdate();
}

void UHealthComponent::OnHealthUpdate()
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (BaseCharacter->GetLocalRole() == ROLE_Authority)
	{
		// when health reaches 0
		if (Health <= 0.f)
		{
			BaseCharacter->HandleDeath();
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

	OnHealthEvent.Broadcast();
}

void UHealthComponent::SetHealth(float InHealth)
{
	Health = FMath::Clamp(InHealth, 0.f, MaxHealth);

	OnHealthUpdate();
}

float UHealthComponent::GetHealth()
{
	return Health;
}

void UHealthComponent::OnRep_MaxHealth()
{
	OnMaxHealthUpdate();
}

void UHealthComponent::OnMaxHealthUpdate()
{
	OnMaxHealthEvent.Broadcast();
}

void UHealthComponent::SetMaxHealth(float InLevel, float InItemMultiplierHealth)
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

	MaxHealth = BaseCharacter->AlzadiGameModeRef->FormulaMaxHealth(InLevel, InItemMultiplierHealth);

	OnMaxHealthUpdate();
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                  AController* InstigatedBy, AActor* DamageCauser)
{
	LastDamage = FMath::Clamp(Damage, 0.f, Health);

	// add damage caused by others to array
	bool bHasCausedDamage = false;

	// analyse if attacker has already dealt damage
	for (TPair<ABaseCharacter*, float>& SingleAttackerDamage : AttackerDamage)
	{
		if (SingleAttackerDamage.Key)
		{
			if (SingleAttackerDamage.Key == Cast<ABaseCharacter>(DamageCauser))
			{
				SingleAttackerDamage.Value += LastDamage;
				bHasCausedDamage = true;
			}
		}
	}

	// add new attacker
	if (!bHasCausedDamage)
	{
		AttackerDamage.Add(Cast<ABaseCharacter>(DamageCauser), LastDamage);
	}
	
	SetHealth(Health - LastDamage);
}

TMap<ABaseCharacter*, float> UHealthComponent::GetAttackerDamage()
{
	return AttackerDamage;
}

void UHealthComponent::ResetAttackerDamage()
{
	AttackerDamage.Empty();
}
