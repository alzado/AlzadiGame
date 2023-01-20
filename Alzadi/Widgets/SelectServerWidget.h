// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Blueprint/UserWidget.h"
#include "SelectServerWidget.generated.h"

class UEditableText;
class UTextBlock;
class UButton;

UCLASS()
class ALZADI_API USelectServerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USelectServerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UTextBlock* Message;

	UPROPERTY(meta=(BindWidget))
	UEditableText* GameServerUrl;
	
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta=(BindWidget))
	UButton* Connect;

	UFUNCTION(Category="Widget")
	void SetMessage(FData Data);

	UFUNCTION(Category="Widget")
	void SelectServerWidgetConnect();

	UFUNCTION(Category="Widget")
	void SelectServerWidgetConnectSuccess(FData Data);

	UFUNCTION(Category="Widget")
	void SelectServerWidgetConnectError(FData Data);
};
