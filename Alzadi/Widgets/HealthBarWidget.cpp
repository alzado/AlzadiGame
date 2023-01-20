// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Health/HealthComponent.h"
#include "Components/ProgressBar.h"


UHealthBarWidget::UHealthBarWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// called every time is set visible, mainly to update others health bars
	SetHealthBar();
}

void UHealthBarWidget::SetupWidget()
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

	BaseCharacterReference->HealthComponent->OnHealthEvent.AddUObject(
		this, &UHealthBarWidget::OnHealthComponentHealthUpdate);
	BaseCharacterReference->HealthComponent->OnMaxHealthEvent.AddUObject(
		this, &UHealthBarWidget::OnHealthComponentMaxHealthUpdate);
}

void UHealthBarWidget::SetBaseCharacterReference(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}
	
	BaseCharacterReference = Character;
}

void UHealthBarWidget::SetHealthBar()
{
	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterReference in %s."), *GetName());
		return;
	}

	if (!BaseCharacterReference->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *BaseCharacterReference->GetName());
		return;
	}

	if (!HealthBarHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthBarHUD in %s."), *GetName());
		return;
	}

	HealthBarHUD->SetPercent(
		BaseCharacterReference->HealthComponent->GetHealth() / BaseCharacterReference->HealthComponent->
		GetMaxHealth());
}

void UHealthBarWidget::OnHealthComponentHealthUpdate()
{
	SetHealthBar();
}

void UHealthBarWidget::OnHealthComponentMaxHealthUpdate()
{
	SetHealthBar();
}
