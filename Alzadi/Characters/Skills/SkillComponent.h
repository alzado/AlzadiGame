// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE(FOnSkillEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALZADI_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// set owner reference
	TObjectPtr<ABaseCharacter> BaseCharacter;

	void SetBaseCharacter(ABaseCharacter* Character);

	// reset
	void ResetComponent();

	// skill

	UPROPERTY(ReplicatedUsing=OnRep_Skill, EditDefaultsOnly, Category="Skills")
	FSkill Skill;

	UFUNCTION()
	void OnRep_Skill();

	void OnSkillUpdate();

	FOnSkillEvent OnSkillEvent;

	void SetSkill(FSkill Skill);

	FSkill GetSkill();
};
