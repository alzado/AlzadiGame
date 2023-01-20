// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Core/Definitions.h"
#include "Blueprint/UserWidget.h"
#include "AlzadiWidget.generated.h"

class UCanvasPanel;
class UButton;
class UCharacterInfoWidget;
class UEquipedWidget;
class AAlzadiPlayerController;
class ABaseCharacter;
class UInventoryPanel;

UCLASS()
class ALZADI_API UAlzadiWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAlzadiWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	// Set Player Controller Reference
	TObjectPtr<AAlzadiPlayerController> AlzadiPlayerControllerReference;

	// set pawn reference
	TObjectPtr<ABaseCharacter> BaseCharacterReference;

	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta=(BindWidget))
	UButton* Logout;

	UFUNCTION()
	void AlzadiWidgetLogout();

	UFUNCTION()
	void AlzadiWidgetLogoutSuccess(FData Data);

	UFUNCTION()
	void AlzadiWidgetLogoutError(FData Data);

	// Character info widget
	UPROPERTY(meta=(BindWidget))
	UCharacterInfoWidget* CharacterInfoWidget;

	// Character Equiped
	UPROPERTY(meta=(BindWidget))
	UEquipedWidget* EquipedWidget;

	// Character Inventory
	UPROPERTY(meta=(BindWidget))
	UInventoryPanel* InventoryPanel;
};
