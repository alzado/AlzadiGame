// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterWidgetComponent.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Interfaces/CharacterWidget.h"

UCharacterWidgetComponent::UCharacterWidgetComponent()
{
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UCharacterWidget> ClassFound(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/WBP_CharacterWidget'"));
	if (ClassFound.Succeeded())
	{
		SetWidgetClass(ClassFound.Class);
	}
}

void UCharacterWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!BaseCharacterReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}


	if (BaseCharacterReference->GetLocalRole() == ROLE_Authority)
	{
		// in server, controlled by server (applies for offline game and AI controllers)
		if (BaseCharacterReference->IsLocallyControlled())
		{
		}
		// in server, not controlled by server
		else
		{
		}
	}
	else
	{
		// in client, controlled by this client
		if (BaseCharacterReference->IsLocallyControlled())
		{
		}
		// for every other clients
		else
		{
		}

		// For all clients
		UCharacterWidget* CharacterWidget = Cast<UCharacterWidget>(GetUserWidgetObject());

		CharacterWidget->SetBaseCharacterRef(BaseCharacterReference);
		CharacterWidget->SetupWidget();
	}
}

void UCharacterWidgetComponent::SetBaseCharacterReference(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	BaseCharacterReference = Character;
}
