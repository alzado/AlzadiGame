// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Alzadi/Core/AlzadiPlayerController.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// AlzadiPlayerControllerReference = Cast<AAlzadiPlayerController>(GetOwningPlayer());
	//
	// Message->SetVisibility(ESlateVisibility::Hidden);
	//
	// Login->OnClicked.AddDynamic(this, &UMainMenuWidget::MainMenuWidgetLogin);
	// Create->OnClicked.AddDynamic(this, &UMainMenuWidget::MainMenuWidgetCreate);
	// Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::MainMenuWidgetExit);
}

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Message in %s."), *GetName());
		return;
	}

	if (!Login)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Login in %s."), *GetName());
		return;
	}

	if (!Create)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Create in %s."), *GetName());
		return;
	}

	if (!Exit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Exit in %s."), *GetName());
		return;
	}

	AlzadiPlayerControllerReference = Cast<AAlzadiPlayerController>(GetOwningPlayer());

	Message->SetVisibility(ESlateVisibility::Hidden);

	Login->OnClicked.AddDynamic(this, &UMainMenuWidget::MainMenuWidgetLogin);
	Create->OnClicked.AddDynamic(this, &UMainMenuWidget::MainMenuWidgetCreate);
	Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::MainMenuWidgetExit);
}

void UMainMenuWidget::SetMessage(FData Data)
{
	if (!Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Message in %s."), *GetName());
		return;
	}
	
	Message->SetText(FText::FromString(Data.Message));
	Message->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::MainMenuWidgetLogin()
{
	if (!AlzadiPlayerControllerReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiPlayerControllerRef in %s."), *GetName());
		return;
	}

	if (!Account)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Account in %s."), *GetName());
		return;
	}

	if (!Password)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Password in %s."), *GetName());
		return;
	}

	AlzadiPlayerControllerReference->CharacterLogin(Account->GetText().ToString(),
	                                                Password->GetText().ToString());
}

void UMainMenuWidget::MainMenuWidgetLoginSuccess(FData Data)
{
	if (!Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Message in %s."), *GetName());
		return;
	}
	
	Message->SetVisibility(ESlateVisibility::Hidden);
	SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::MainMenuWidgetLoginError(FData Data)
{
	SetMessage(Data);
}

void UMainMenuWidget::MainMenuWidgetCreate()
{
	if (!AlzadiPlayerControllerReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiPlayerControllerRef in %s."), *GetName());
		return;
	}

	if (!Account)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Account in %s."), *GetName());
		return;
	}

	if (!Password)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Password in %s."), *GetName());
		return;
	}

	AlzadiPlayerControllerReference->CharacterCreate(Account->GetText().ToString(),
	                                                 Password->GetText().ToString());
}

void UMainMenuWidget::MainMenuWidgetCreateSuccess(FData Data)
{
	SetMessage(Data);
}

void UMainMenuWidget::MainMenuWidgetCreateError(FData Data)
{
	SetMessage(Data);
}

void UMainMenuWidget::MainMenuWidgetExit()
{
	if (!AlzadiPlayerControllerReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiPlayerControllerRef in %s."), *GetName());
		return;
	}

	AlzadiPlayerControllerReference->DisconnectFromServer();
}
