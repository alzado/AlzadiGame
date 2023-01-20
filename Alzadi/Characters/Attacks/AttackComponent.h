// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE(FOnBaseAttackEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALZADI_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Set owner
	TObjectPtr<ABaseCharacter> BaseCharacter;
	
	void SetBaseCharacter(ABaseCharacter* Character);

	// damage multipliers
	UPROPERTY(Replicated)
	FDamageMultiplier DamageMultiplier;

	void SetDamageMultiplier();

	FDamageMultiplier GetDamageMultiplier();

	// Attack timer
	FTimerHandle TimerHandle;

	// Attack rate
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float BaseAttackRate = 3.f;

	// Start attack
	void BaseAttackLoop();

	// base attack
	UPROPERTY(ReplicatedUsing=OnRep_BaseAttack)
	FAttack BaseAttack;

	void SetBaseAttack(FAttack Attack);

	FAttack GetBaseAttack();

	UFUNCTION()
	void OnRep_BaseAttack();

	void OnBaseAttackUpdate();

	FOnBaseAttackEvent OnBaseAttackEvent;

	// updates
	void OnEquipmentComponentEquipedUpdate();

	void OnExperienceComponentLevelUpdate();

	void OnSkillComponentSkillUpdate();
};
