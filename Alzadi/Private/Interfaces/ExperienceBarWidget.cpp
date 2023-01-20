// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/ExperienceBarWidget.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Experience/ExperienceComponent.h"
#include "Components/ProgressBar.h"


void UExperienceBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetExperienceBarHUD();
}

void UExperienceBarWidget::SetupWidget()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterRef in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	BaseCharacterRef->ExperienceComponent->OnExperienceEvent.AddUObject(
		this, &UExperienceBarWidget::OnExperienceComponentExperienceUpdate);

	BaseCharacterRef->ExperienceComponent->OnLevelEvent.AddUObject(
		this, &UExperienceBarWidget::OnExperienceComponentLevelUpdate);
}

void UExperienceBarWidget::SetBaseCharacterRef(ABaseCharacter* InBaseCharacter)
{
	if (!InBaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing InBaseCharacter in %s."), *GetName());
		return;
	}

	BaseCharacterRef = InBaseCharacter;
}

void UExperienceBarWidget::SetExperienceBarHUD()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (BaseCharacterRef->ExperienceComponent->ExpLevelUp - BaseCharacterRef->ExperienceComponent->ExpLevelDown == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Dividing by 0 in %s."), *GetName());
		return;
	}
	
	ExperienceBarHUD->SetPercent(
		(BaseCharacterRef->ExperienceComponent->GetExperience() - BaseCharacterRef->ExperienceComponent->GetExpLevelDown()) / (
			BaseCharacterRef->ExperienceComponent->GetExpLevelUp() - BaseCharacterRef->ExperienceComponent->GetExpLevelDown()));
}

void UExperienceBarWidget::OnExperienceComponentExperienceUpdate()
{
	SetExperienceBarHUD();
}

void UExperienceBarWidget::OnExperienceComponentLevelUpdate()
{
	SetExperienceBarHUD();
}
