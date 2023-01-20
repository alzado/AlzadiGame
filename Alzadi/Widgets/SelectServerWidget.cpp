// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectServerWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


USelectServerWidget::USelectServerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USelectServerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Message->SetVisibility(ESlateVisibility::Hidden);
	//
	// Connect->OnClicked.AddDynamic(this, &USelectServerWidget::SelectServerWidgetConnect);
}

void USelectServerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Message in %s."), *GetName());
		return;
	}

	if (!Connect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Connect in %s."), *GetName());
		return;
	}

	Message->SetVisibility(ESlateVisibility::Hidden);

	Connect->OnClicked.AddDynamic(this, &USelectServerWidget::SelectServerWidgetConnect);
}

void USelectServerWidget::SetMessage(FData Data)
{
	if (!Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Message in %s."), *GetName());
		return;
	}
	
	Message->SetText(FText::FromString(Data.Message));
	Message->SetVisibility(ESlateVisibility::Visible);
}

void USelectServerWidget::SelectServerWidgetConnect()
{
	UGameplayStatics::OpenLevel(this, FName(*GameServerUrl->GetText().ToString()));
}

void USelectServerWidget::SelectServerWidgetConnectSuccess(FData Data)
{
	SetVisibility(ESlateVisibility::Hidden);
}

void USelectServerWidget::SelectServerWidgetConnectError(FData Data)
{
	SetMessage(Data);
}
