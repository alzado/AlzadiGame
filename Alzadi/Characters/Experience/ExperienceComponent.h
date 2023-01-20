// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE(FOnExperienceEvent);
DECLARE_MULTICAST_DELEGATE(FOnLevelEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALZADI_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UExperienceComponent();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Set pawn Reference
	TObjectPtr<ABaseCharacter> BaseCharacter;

	void SetBaseCharacter(ABaseCharacter* Character);

	// reset component
	void ResetExperienceComponent();

	// Experience variables and functions
	UPROPERTY(ReplicatedUsing=OnRep_Experience, EditDefaultsOnly, Category="Experience")
	float Experience = 0.f;

	UPROPERTY(Replicated)
	float ExpLevelUp = 0.f;

	float GetExpLevelUp();

	UPROPERTY(Replicated)
	float ExpLevelDown = 0.f;
	
	float GetExpLevelDown();

	void SetExpLevelUpAndDown();

	UFUNCTION()
	void OnRep_Experience();

	void OnExperienceUpdate();

	FOnExperienceEvent OnExperienceEvent;

	void SetExperience(float InExperience);

	float GetExperience();

	UPROPERTY(EditDefaultsOnly, Category="Experience")
	float ExperienceLossOnDeath = 0.1f;

	// Level variables and functions
	UPROPERTY(ReplicatedUsing=OnRep_Level, VisibleAnywhere, Category="Level")
	float Level = 0.f;

	UFUNCTION()
	void OnRep_Level();

	void OnLevelUpdate();

	FOnLevelEvent OnLevelEvent;

	void SetLevel(float InLevel);

	float GetLevel();

	// kill experience
	UPROPERTY(Replicated)
	float KillExperience = 0.f;

	void SetKillExperience(float InKillExperience);

	float GetKillExperience();

	void GiveExperienceWhenKilled();
};
