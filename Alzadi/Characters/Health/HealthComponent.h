// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ABaseCharacter;
class UWidgetComponent;

DECLARE_MULTICAST_DELEGATE(FOnHealthEvent);
DECLARE_MULTICAST_DELEGATE(FOnMaxHealthEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALZADI_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UHealthComponent();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	// Set pawn reference
	TObjectPtr<ABaseCharacter> BaseCharacter;

	void SetBaseCharacter(ABaseCharacter* Character);

	// Reset component
	void ResetComponent();

	// Current health

	UPROPERTY(ReplicatedUsing=OnRep_Health, EditDefaultsOnly, Category="Health")
	float Health = 0.f;

	UFUNCTION()
	void OnRep_Health();

	void OnHealthUpdate();

	FOnHealthEvent OnHealthEvent;

	void SetHealth(float InHealth);

	float GetHealth();

	// Max health

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, EditDefaultsOnly, Category="Health")
	float MaxHealth = 0.f;

	UFUNCTION()
	void OnRep_MaxHealth();

	void OnMaxHealthUpdate();

	FOnMaxHealthEvent OnMaxHealthEvent;

	void SetMaxHealth(float InLevel, float InItemMultiplierHealth);

	float GetMaxHealth();

	// Damage

	UFUNCTION(Category = "Damage")
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated)
	float LastDamage = 0.f;

	TMap<ABaseCharacter*, float> AttackerDamage;

	TMap<ABaseCharacter*, float> GetAttackerDamage();

	void ResetAttackerDamage();
};
