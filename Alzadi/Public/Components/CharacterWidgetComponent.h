// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

class ABaseCharacter;
class UHealthBarWidget;

UCLASS()
class ALZADI_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UCharacterWidgetComponent();

protected:
	virtual void BeginPlay() override;

public:
	// set pawn reference
	TObjectPtr<ABaseCharacter> BaseCharacterReference;
	
	void SetBaseCharacterReference(ABaseCharacter* Character);
};
