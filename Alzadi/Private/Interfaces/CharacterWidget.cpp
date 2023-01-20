// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/CharacterWidget.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Health/HealthComponent.h"
#include "Alzadi/Widgets/HealthBarWidget.h"
#include "Components/TextBlock.h"


void UCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!CharacterName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing CharacterName in %s."), *GetName());
		return;
	}

	CharacterName->SetText(FText::FromName(BaseCharacterRef->GetCharacterName()));
}

void UCharacterWidget::SetupWidget()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!HealthBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthBarWidget in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *BaseCharacterRef->GetName());
		return;
	}

	HealthBarWidget->SetBaseCharacterReference(BaseCharacterRef);
	HealthBarWidget->SetupWidget();

	BaseCharacterRef->HealthComponent->OnHealthEvent.AddUObject(
		this, &UCharacterWidget::OnHealthComponentHealthUpdate);
}

void UCharacterWidget::SetBaseCharacterRef(ABaseCharacter* BaseCharacter)
{
	if (!BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}
	
	BaseCharacterRef = BaseCharacter;
}

void UCharacterWidget::OnHealthComponentHealthUpdate()
{
	if (!HitPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HitPoints in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *BaseCharacterRef->GetName());
		return;
	}

	if (!HitPointsAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HitPointsAnimation in %s."), *GetName());
		return;
	}

	HitPoints->SetText(FText::FromString(FString::SanitizeFloat(-BaseCharacterRef->HealthComponent->LastDamage)));
	PlayAnimation(HitPointsAnimation);
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UCharacterWidget::StopTimer);
	BaseCharacterRef->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.f, false);
}

void UCharacterWidget::StopTimer()
{
	if (!HitPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HitPoints in %s."), *GetName());
		return;
	}

	HitPoints->SetText(FText::FromString(TEXT("")));
}
