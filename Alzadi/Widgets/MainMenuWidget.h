// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class AAlzadiPlayerController;
class UTextBlock;
class UEditableText;
class UButton;

UCLASS()
class ALZADI_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	// Set owner reference
	TObjectPtr<AAlzadiPlayerController> AlzadiPlayerControllerReference;

	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UTextBlock* Message;
	
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UEditableText* Account;

	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UEditableText* Password;

	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UButton* Login;

	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UButton* Create;

	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UButton* Exit;

	UFUNCTION(Category="Widget")
	void SetMessage(FData Data);

	UFUNCTION(Category="Widget")
	void MainMenuWidgetLogin();

	UFUNCTION(Category="Widget")
	void MainMenuWidgetLoginSuccess(FData Data);

	UFUNCTION(Category="Widget")
	void MainMenuWidgetLoginError(FData Data);

	UFUNCTION(Category="Widget")
	void MainMenuWidgetCreate();

	UFUNCTION(Category="Widget")
	void MainMenuWidgetCreateSuccess(FData Data);

	UFUNCTION(Category="Widget")
	void MainMenuWidgetCreateError(FData Data);

	UFUNCTION(Category="Widget")
	void MainMenuWidgetExit();
};
