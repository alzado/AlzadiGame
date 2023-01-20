// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Definitions.h"
#include "GameFramework/PlayerController.h"
#include "AlzadiPlayerController.generated.h"

class AAlzadiGameMode;
class AAlzadiHUD;
class ABaseCharacter;
class IWebSocket;

UCLASS()
class ALZADI_API AAlzadiPlayerController : public APlayerController
{
	GENERATED_BODY()

	AAlzadiPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	TObjectPtr<AAlzadiGameMode> AlzadiGameMode;

	UPROPERTY()
	ABaseCharacter* BaseCharacterRef;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category="Login")
	void CharacterLogin(const FString& Account, const FString& Password);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category="Create")
	void CharacterCreate(const FString& Account, const FString& Password);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category="Logout")
	void CharacterLogout();
	
	void ConnectToServer();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category="Web Server")
	void DisconnectFromServer();

	UFUNCTION(Category="Web Server")
	void ConnectionSuccessServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Web Server")
	void ConnectionSuccessClient(FData Data);

	UFUNCTION(Category="Web Server")
	void ConnectionErrorServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Web Server")
	void ConnectionErrorClient(FData Data);

	UFUNCTION(Category="Web Server")
	void DisconnectionSuccessServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Web Server")
	void DisconnectionSuccessClient(FData Data);

	UFUNCTION(Category="Web Server")
	void DisconnectionErrorServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Web Server")
	void DisconnectionErrorClient(FData Data);

	UFUNCTION(Category="Login")
	void CharacterLoginSuccessServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Login")
	void CharacterLoginSuccessClient(FData Data);

	UFUNCTION(Category="Login")
	void CharacterLoginErrorServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Login")
	void CharacterLoginErrorClient(FData Data);

	UFUNCTION(Category="Logout")
	void CharacterLogoutSuccessServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Logout")
	void CharacterLogoutSuccessClient(FData Data);

	UFUNCTION(Category="Create")
	void CharacterCreateSuccessServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Create")
	void CharacterCreateSuccessClient(FData Data);

	UFUNCTION(Category="Create")
	void CharacterCreateErrorServer(FData Data);

	UFUNCTION(Client, Reliable, Category="Create")
	void CharacterCreateErrorClient(FData Data);

	UFUNCTION(Category="Server Save")
	void UploadCharacterInfoSuccessServer(FData Data);

	UFUNCTION(Category="ServerSave")
	void UploadCharacterInfoErrorServer(FData Data);

	// inputs
	void MoveForward(float Value);
	void MoveRight(float Value);

	// mouse hover
	UPROPERTY()
	ABaseCharacter* HoveredTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Target")
	UMaterial* HoveredTargetMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Target")
	UMaterial* LockedTargetMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Target")
	UMaterial* NoTargetMaterial;

	UFUNCTION(Category="Hover")
	void MouseHoverClient();

	// Lock target to attack
	UFUNCTION(Category="Click")
	void LeftMouseButtonClient();

	UFUNCTION(Category="Target")
	void MouseLockedClient();

	// activate/deactivate controller and character
	UPROPERTY(Replicated)
	bool bIsConnected = false;

	void HideCharacter(bool bHideCharacter);

	// upload character info
	FTimerHandle UploadCharacterInformationTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Server Save")
	float UploadRate = 10.f;

	void UploadCharacterInfo();

	FData PackCharacterInfo();

	// Widgets

	TObjectPtr<AAlzadiHUD> AlzadiHUD;

	UFUNCTION(Category="Web Server")
	void SendMessage(FData Data);

private:
	TSharedPtr<IWebSocket> WebSocket;

	UPROPERTY(EditDefaultsOnly, Category="Queue Server")
	FName QueueServerUrl;
};
