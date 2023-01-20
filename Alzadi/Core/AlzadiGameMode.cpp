// Fill out your copyright notice in the Description page of Project Settings.


#include "AlzadiGameMode.h"

#include "Kismet/GameplayStatics.h"


void AAlzadiGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AAlzadiGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCountClass && ClassToCount)
	{
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToCount, AllActors);

		UE_LOG(LogTemp, Warning, TEXT("%d actors of class %s"), AllActors.Num(), *ClassToCount->GetName());
	}
}

FName AAlzadiGameMode::GetWebServerUrl()
{
	return WebServerUrl;
}
