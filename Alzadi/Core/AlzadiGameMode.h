// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alzadi/Items/AlzadiDataAsset.h"
#include "GameFramework/GameMode.h"
#include "AlzadiGameMode.generated.h"

UCLASS()
class ALZADI_API AAlzadiGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Web Server")
	FName WebServerUrl;

public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category=Debug)
	bool bCountClass = false;
	
	UPROPERTY(EditDefaultsOnly, Category=Debug)
	TSubclassOf<AActor> ClassToCount;
	
	FName GetWebServerUrl();

	// Items Database
	UPROPERTY(EditDefaultsOnly, Category="Item System")
	UAlzadiDataAsset* AlzadiDataAsset;

	// game formulas

	FORCEINLINE float FormulaLevelExperience(float InLevel)
	{
		return 50.f / 3.f * (InLevel * InLevel * InLevel - 6.f * InLevel * InLevel + 17.f * InLevel - 12.f);
	}

	FORCEINLINE float FormulaMaxHealth(float Level, float ItemMultiplierHealth) { return 100.f + Level * 10.f; }

	FORCEINLINE float FormulaLevelDamage(float Level) { return Level; }

	FORCEINLINE float FormulaSkillDamage(float Skill) { return Skill; }

	FORCEINLINE float FormulaSpeedLevel(float Level, float ItemMultiplierSpeed) { return 220.f + 2.f * (Level - 1.f); }
};
