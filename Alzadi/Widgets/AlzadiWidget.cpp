// Fill out your copyright notice in the Description page of Project Settings.


#include "AlzadiWidget.h"

#include "CharacterInfoWidget.h"
#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Core/AlzadiPlayerController.h"
#include "Components/Button.h"


UAlzadiWidget::UAlzadiWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UAlzadiWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// AlzadiPlayerControllerReference = Cast<AAlzadiPlayerController>(GetOwningPlayer());
	// BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	//
	// Logout->OnClicked.AddDynamic(this, &UAlzadiWidget::AlzadiWidgetLogout);

	// CharacterInfoWidget->SetBaseCharacterReference(BaseCharacterReference);
	// EquipedWidget->SetBaseCharacterReference(BaseCharacterReference);
	// InventoryPanel->SetBaseCharacterReference(BaseCharacterReference);
}

void UAlzadiWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Super::NativeConstruct();

	AlzadiPlayerControllerReference = Cast<AAlzadiPlayerController>(GetOwningPlayer());
	BaseCharacterReference = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	Logout->OnClicked.AddDynamic(this, &UAlzadiWidget::AlzadiWidgetLogout);
}

void UAlzadiWidget::AlzadiWidgetLogout()
{
	if (!AlzadiPlayerControllerReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiPlayerControllerReference in %s."), *GetName());
		return;
	}

	AlzadiPlayerControllerReference->CharacterLogout();
}

void UAlzadiWidget::AlzadiWidgetLogoutSuccess(FData Data)
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UAlzadiWidget::AlzadiWidgetLogoutError(FData Data)
{
}
