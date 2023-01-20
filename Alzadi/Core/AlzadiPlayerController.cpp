// Fill out your copyright notice in the Description page of Project Settings.


#include "AlzadiPlayerController.h"

#include "AlzadiGameMode.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"
#include "WebSocketsModule.h"
#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Experience/ExperienceComponent.h"
#include "Alzadi/Characters/Health/HealthComponent.h"
#include "Alzadi/Characters/Inventory/EquipmentComponent.h"
#include "Alzadi/Characters/Skills/SkillComponent.h"
#include "Alzadi/Widgets/AlzadiHUD.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Alzadi/Widgets/AlzadiWidget.h"
#include "Alzadi/Widgets/MainMenuWidget.h"
#include "Alzadi/Widgets/SelectServerWidget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


AAlzadiPlayerController::AAlzadiPlayerController()
{
	bReplicates = true;
}

// for client side functions
void AAlzadiPlayerController::BeginPlay()
{
	Super::BeginPlay();


	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
			AlzadiHUD = Cast<AAlzadiHUD>(GetHUD());
			BaseCharacterRef = Cast<ABaseCharacter>(GetPawn());
			HideCharacter(true);

			BaseCharacterRef->SetAlzadiPlayerControllerRef(this);
			SetShowMouseCursor(true);
		}
		else
		{
		}
	}
	else
	{
		if (IsLocalController())
		{
			AlzadiHUD = Cast<AAlzadiHUD>(GetHUD());
			BaseCharacterRef = Cast<ABaseCharacter>(GetPawn());
			HideCharacter(true);

			BaseCharacterRef->SetAlzadiPlayerControllerRef(this);
			SetShowMouseCursor(true);
		}
		else
		{
		}
	}
}

void AAlzadiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ROLE_Authority)
	{
		AlzadiGameMode = Cast<AAlzadiGameMode>(UGameplayStatics::GetGameMode(this));
		BaseCharacterRef = Cast<ABaseCharacter>(GetPawn());
		HideCharacter(true);
		BaseCharacterRef->SetAlzadiGameModeRef(AlzadiGameMode);
		BaseCharacterRef->SetAlzadiPlayerControllerRef(this);

		if (IsLocalController())
		{
		}
		else
		{
			ConnectToServer();

			// Setup web server save
			FTimerDelegate UploadCharacterInformationTimerDelegate = FTimerDelegate::CreateUObject(
				this, &AAlzadiPlayerController::UploadCharacterInfo);
			GetWorldTimerManager().SetTimer(UploadCharacterInformationTimerHandle,
			                                UploadCharacterInformationTimerDelegate, UploadRate, true);
		}
	}
	else
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
}

void AAlzadiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move Forward / Backward", this, &AAlzadiPlayerController::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &AAlzadiPlayerController::MoveRight);

	InputComponent->BindAction(TEXT("LeftMouseButton"), IE_Pressed, this,
	                           &AAlzadiPlayerController::LeftMouseButtonClient);
}

void AAlzadiPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
	else
	{
		if (IsLocalController())
		{
			if (bIsConnected)
			{
				MouseHoverClient();
			}
		}
		else
		{
		}
	}
}

void AAlzadiPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAlzadiPlayerController, bIsConnected);
}

void AAlzadiPlayerController::SendMessage(FData Data)
{
	if (!WebSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing WebSocket in %s."), *GetName());
		return;
	}

	if (WebSocket->IsConnected())
	{
		FString MessageString;
		FJsonObjectConverter::UStructToJsonObjectString(Data, MessageString, 0, 0);

		WebSocket->Send(MessageString);
	}
}


void AAlzadiPlayerController::ConnectToServer()
{
	// Test if module is active or not
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	// Connect to web server
	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://127.0.0.1:8080");

	// Message when successfully connected to server
	WebSocket->OnConnected().AddLambda([this]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Successfully connected");

		FData Data;
		Data.Message = TEXT("Success");

		ConnectionSuccessServer(Data);
	});

	// Message when initial connection failed
	WebSocket->OnConnectionError().AddLambda([this](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, Error);

		FData Data;
		Data.Message = Error;

		ConnectionErrorServer(Data);
	});

	// Message when connection is closed, due to error or not
	WebSocket->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, bWasClean ? FColor::Green : FColor::Red,
		                                 "Connection closed: " + Reason);
		FData Data;
		Data.Message = Reason;

		// Server Work
		if (bWasClean)
		{
			DisconnectionSuccessServer(Data);
		}
		else
		{
			DisconnectionErrorServer(Data);
		}
	});

	// Action when data is received
	WebSocket->OnMessage().AddLambda([this](const FString& MessageString)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Received message: " + MessageString);

		FData Data;
		FJsonObjectConverter::JsonObjectStringToUStruct(MessageString,
		                                                &Data, 0, 0);

		if (Data.Topic == TEXT("characterLoginSuccess"))
		{
			CharacterLoginSuccessServer(Data);
		}
		else if (Data.Topic == TEXT("characterLoginError"))
		{
			CharacterLoginErrorServer(Data);
		}
		else if (Data.Topic == TEXT("characterLogoutSuccess"))
		{
			CharacterLogoutSuccessServer(Data);
		}
		else if (Data.Topic == TEXT("characterCreateSuccess"))
		{
			CharacterCreateSuccessServer(Data);
		}
		else if (Data.Topic == TEXT("characterCreateError"))
		{
			CharacterCreateErrorServer(Data);
		}
		else if (Data.Topic == TEXT("uploadCharacterInfoSuccess"))
		{
			UploadCharacterInfoSuccessServer(Data);
		}
		else if (Data.Topic == TEXT("uploadCharacterInfoError"))
		{
			UploadCharacterInfoErrorServer(Data);
		}
	});

	// Action when data is sent
	WebSocket->OnMessageSent().AddLambda([this](const FString& MessageString)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "Sent message: " + MessageString);
	});

	// Connect to web server
	WebSocket->Connect();
}

void AAlzadiPlayerController::ConnectionSuccessServer(FData Data)
{
	ConnectionSuccessClient(Data);
}

void AAlzadiPlayerController::ConnectionSuccessClient_Implementation(FData Data)
{
	if (!AlzadiHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiHUD in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->SelectServerWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SelectServerWidget in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidget in %s."), *GetName());
		return;
	}

	AlzadiHUD->SelectServerWidget->SelectServerWidgetConnectSuccess(Data);
	AlzadiHUD->MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}


void AAlzadiPlayerController::ConnectionErrorServer(FData Data)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
		}
		else
		{
			ConnectionErrorClient(Data);
		}
	}
	else
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
}

void AAlzadiPlayerController::ConnectionErrorClient_Implementation(FData Data)
{
	UGameplayStatics::OpenLevel(this, QueueServerUrl);
}


void AAlzadiPlayerController::DisconnectFromServer_Implementation()
{
	if (!WebSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing WebSocket in %s."), *GetName());
		return;
	}

	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
}

bool AAlzadiPlayerController::DisconnectFromServer_Validate()
{
	return true;
}

void AAlzadiPlayerController::DisconnectionSuccessServer(FData Data)
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	BaseCharacterRef->HandleDestruction(Data);
	DisconnectionSuccessClient(Data);
}


void AAlzadiPlayerController::DisconnectionSuccessClient_Implementation(FData Data)
{
	UGameplayStatics::OpenLevel(this,TEXT("EntryMap"));
}

void AAlzadiPlayerController::DisconnectionErrorServer(FData Data)
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	BaseCharacterRef->HandleDestruction(Data);
	DisconnectionErrorClient(Data);
}

void AAlzadiPlayerController::DisconnectionErrorClient_Implementation(FData Data)
{
	UGameplayStatics::OpenLevel(this,TEXT("EntryMap"));
}


void AAlzadiPlayerController::CharacterLogin_Implementation(const FString& Account, const FString& Password)
{
	FData Data;
	Data.Topic = "characterLogin";
	Data.Content.AlzadiAccount.Account = Account;
	Data.Content.AlzadiAccount.Password = Password;
	SendMessage(Data);
}

bool AAlzadiPlayerController::CharacterLogin_Validate(const FString& Account, const FString& Password)
{
	return true;
}

void AAlzadiPlayerController::CharacterLoginSuccessServer(FData Data)
{
	HideCharacter(false);

	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	// not in components
	BaseCharacterRef->SetCharacterType(Data.Content.AlzadiCharacter.CharacterType);
	BaseCharacterRef->SetCharacterName(Data.Content.AlzadiCharacter.Name);
	BaseCharacterRef->SetSpawnPoint(Data.Content.AlzadiCharacter.SpawnPoint);

	// Set experience
	BaseCharacterRef->ExperienceComponent->SetExperience(Data.Content.AlzadiCharacter.Experience);

	// Set Skills
	BaseCharacterRef->SkillComponent->SetSkill(Data.Content.AlzadiCharacter.Skill);

	// Set items
	BaseCharacterRef->EquipmentComponent->SetEquipedFromData(Data.Content.AlzadiCharacter.Equipment.Set);
	BaseCharacterRef->EquipmentComponent->SetInventoryFromData(Data.Content.AlzadiCharacter.Equipment.Inventory);

	// Set MaxHealth based on level, vocation and items, and currentHealth based on web server
	BaseCharacterRef->HealthComponent->SetMaxHealth(BaseCharacterRef->ExperienceComponent->GetLevel(), 0.f);
	BaseCharacterRef->HealthComponent->SetHealth(Data.Content.AlzadiCharacter.Health);

	// put character in place
	BaseCharacterRef->SetActorLocation(Data.Content.AlzadiCharacter.Location.ToFVector());

	CharacterLoginSuccessClient(Data);
}


void AAlzadiPlayerController::CharacterLoginSuccessClient_Implementation(FData Data)
{
	if (!AlzadiHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiHUD in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidget in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->AlzadiWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiWidget in %s."), *GetName());
		return;
	}

	HideCharacter(false);
	AlzadiHUD->MainMenuWidget->MainMenuWidgetLoginSuccess(Data);
	AlzadiHUD->AlzadiWidget->SetVisibility(ESlateVisibility::Visible);
}

void AAlzadiPlayerController::CharacterLoginErrorServer(FData Data)
{
	CharacterLoginErrorClient(Data);
}


void AAlzadiPlayerController::CharacterLoginErrorClient_Implementation(FData Data)
{
	if (!AlzadiHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiHUD in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidget in %s."), *GetName());
		return;
	}

	AlzadiHUD->MainMenuWidget->MainMenuWidgetLoginError(Data);
}

void AAlzadiPlayerController::CharacterCreate_Implementation(const FString& Account, const FString& Password)
{
	if (!AlzadiGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiGameMode in %s."), *GetName());
		return;
	}

	if (!AlzadiGameMode->AlzadiDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiDataAsset in %s."), *GetName());
		return;
	}

	FData Data;
	Data.Topic = "characterCreate";
	Data.Content.AlzadiAccount.Account = Account;
	Data.Content.AlzadiAccount.Password = Password;

	Data.Content.AlzadiCharacter = AlzadiGameMode->AlzadiDataAsset->NewPlayerGameInfo;


	// name
	Data.Content.AlzadiCharacter.Name = FName(*Account);

	// Initial stats
	// Data.Content.AlzadiCharacter.Experience = AlzadiGameMode->AlzadiDataAsset->NewPlayerGameInfo.Experience;
	// Data.Content.AlzadiCharacter.Skill = AlzadiGameMode->AlzadiDataAsset->NewPlayerGameInfo.Skill;
	Data.Content.AlzadiCharacter.Health = AlzadiGameMode->FormulaMaxHealth(1, 0.f);

	// initial home town
	// Data.Content.AlzadiCharacter.SpawnPoint = AlzadiGameMode->AlzadiDataAsset->NewPlayerGameInfo.SpawnPoint;

	// Initial position
	Data.Content.AlzadiCharacter.Location = Data.Content.AlzadiCharacter.SpawnPoint;

	// Initial items
	// FItem InitialEquiped;
	// InitialEquiped.Id = AlzadiGameMode->AlzadiDataAsset->BlankItemId;
	// InitialEquiped.Quantity = 1;
	// Data.Content.AlzadiCharacter.Equipment.Set.Add(InitialEquiped);
	// Data.Content.AlzadiCharacter.Equipment.Set.Add(InitialEquiped);
	// Data.Content.AlzadiCharacter.Equipment.Set.Add(InitialEquiped);
	// Data.Content.AlzadiCharacter.Equipment.Set.Add(InitialEquiped);
	// Data.Content.AlzadiCharacter.Equipment.Set.Add(InitialEquiped);
	// Data.Content.AlzadiCharacter.Equipment.Set.Add(InitialEquiped);

	// for (FItem& WebServerItem : AlzadiGameMode->AlzadiDataAsset->NewPlayerGameInfo.Equipment.Inventory)
	// {
	// 	Data.Content.AlzadiCharacter.Equipment.Inventory.Add(WebServerItem);
	// }

	SendMessage(Data);
}

bool AAlzadiPlayerController::CharacterCreate_Validate(const FString& Account, const FString& Password)
{
	return true;
}

void AAlzadiPlayerController::CharacterCreateSuccessServer(FData Data)
{
	CharacterCreateSuccessClient(Data);
}


void AAlzadiPlayerController::CharacterCreateSuccessClient_Implementation(FData Data)
{
	if (!AlzadiHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiHUD in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidget in %s."), *AlzadiHUD->GetName());
		return;
	}

	AlzadiHUD->MainMenuWidget->MainMenuWidgetCreateSuccess(Data);
}

void AAlzadiPlayerController::CharacterCreateErrorServer(FData Data)
{
	CharacterCreateErrorClient(Data);
}

void AAlzadiPlayerController::CharacterCreateErrorClient_Implementation(FData Data)
{
	if (!AlzadiHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiHUD in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidget in %s."), *AlzadiHUD->GetName());
		return;
	}

	AlzadiHUD->MainMenuWidget->MainMenuWidgetCreateError(Data);
}

void AAlzadiPlayerController::CharacterLogout_Implementation()
{
	FData Data = PackCharacterInfo();
	Data.Topic = "characterLogout";
	SendMessage(Data);
}

bool AAlzadiPlayerController::CharacterLogout_Validate()
{
	return true;
}

void AAlzadiPlayerController::CharacterLogoutSuccessServer(FData Data)
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacterRef in %s."), *GetName());
		return;
	}

	DisconnectFromServer();
	// Cast<AConnectedCharacter>(GetPawn())->LoginLogout(false);
	//
	// Cast<AConnectedCharacter>(GetPawn())->SetActorLocation(FVector(0.f, 0.f, 1000.f));
	// Cast<AConnectedCharacter>(GetPawn())->ExperienceComponent->ResetExperienceComponent();
	// Cast<AConnectedCharacter>(GetPawn())->SkillComponent->ResetComponent();
	// Cast<AConnectedCharacter>(GetPawn())->EquipmentComponent->ResetComponent();
	// Cast<AConnectedCharacter>(GetPawn())->HealthComponent->ResetComponent();
	//
	// CharacterLogoutSuccessClient(Data);
}


void AAlzadiPlayerController::CharacterLogoutSuccessClient_Implementation(FData Data)
{
	if (!AlzadiHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiHUD in %s."), *GetName());
		return;
	}

	if (!AlzadiHUD->AlzadiWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiWidget in %s."), *AlzadiHUD->GetName());
		return;
	}

	if (!AlzadiHUD->MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing MainMenuWidget in %s."), *AlzadiHUD->GetName());
		return;
	}

	HideCharacter(true);
	AlzadiHUD->AlzadiWidget->AlzadiWidgetLogoutSuccess(Data);
	AlzadiHUD->MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void AAlzadiPlayerController::UploadCharacterInfoSuccessServer(FData Data)
{
	UE_LOG(LogTemp, Display, TEXT("%s info was saved successfully."), *GetName());
}


void AAlzadiPlayerController::UploadCharacterInfoErrorServer(FData Data)
{
	UE_LOG(LogTemp, Display, TEXT("%s info could not be saved."), *GetName());
}

void AAlzadiPlayerController::MoveForward(float Value)
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	BaseCharacterRef->MoveForward(Value);
}

void AAlzadiPlayerController::MoveRight(float Value)
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	BaseCharacterRef->MoveRight(Value);
}

void AAlzadiPlayerController::HideCharacter(bool bHideCharacter)
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	BaseCharacterRef->HideCharacter(bHideCharacter);

	if (GetLocalRole() == ROLE_Authority)
	{
		bIsConnected = !bHideCharacter;

		if (IsLocalController())
		{
		}
		else
		{
		}
	}
	else
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}

	// called everywhere

	if (bHideCharacter)
	{
		DisableInput(this);
	}
	else
	{
		EnableInput(this);
	}
}

void AAlzadiPlayerController::UploadCharacterInfo()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
		}
		else
		{
			if (bIsConnected)
			{
				FData Data = PackCharacterInfo();
				Data.Topic = "uploadCharacterInfo";

				SendMessage(Data);
			}
		}
	}
}

FData AAlzadiPlayerController::PackCharacterInfo()
{
	FData Data;

	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return Data;
	}

	if (!BaseCharacterRef->ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return Data;
	}

	if (!BaseCharacterRef->HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return Data;
	}

	if (!BaseCharacterRef->SkillComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkillComponent in %s."), *GetName());
		return Data;
	}

	if (!BaseCharacterRef->EquipmentComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing EquipmentComponent in %s."), *GetName());
		return Data;
	}

	// not in components
	Data.Content.AlzadiCharacter.CharacterType = BaseCharacterRef->GetCharacterType();
	Data.Content.AlzadiCharacter.Name = BaseCharacterRef->GetCharacterName();
	Data.Content.AlzadiCharacter.SpawnPoint = BaseCharacterRef->GetSpawnPoint();

	FLocation Location;
	Location.FromFVector(BaseCharacterRef->GetActorLocation());
	Data.Content.AlzadiCharacter.Location = Location;

	// Current Experience
	Data.Content.AlzadiCharacter.Experience = BaseCharacterRef->ExperienceComponent->GetExperience();

	// Current Health
	Data.Content.AlzadiCharacter.Health = BaseCharacterRef->HealthComponent->GetHealth();

	// Current Skills
	Data.Content.AlzadiCharacter.Skill = BaseCharacterRef->SkillComponent->GetSkill();

	// Current Equipment
	Data.Content.AlzadiCharacter.Equipment.Set = BaseCharacterRef->EquipmentComponent->GetEquipedData();
	Data.Content.AlzadiCharacter.Equipment.Inventory = BaseCharacterRef->EquipmentComponent->GetInventoryData();

	return Data;
}

void AAlzadiPlayerController::MouseHoverClient()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	// Get hovered actor
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Pawn, false, HitResult);

	// Has to be a base character 
	if (Cast<ABaseCharacter>(HitResult.GetActor()))
	{
		// Clean hovered client  
		if (HoveredTarget)
		{
			// only if it is not locked target
			if (HoveredTarget != BaseCharacterRef->LockedTarget)
			{
				if (HoveredTarget->PlaneComponent)
				{
					if (HoveredTarget->PlaneComponent->GetStaticMesh())
					{
						HoveredTarget->PlaneComponent->SetMaterial(0, NoTargetMaterial);
						HoveredTarget = nullptr;
					}
				}
			}
		}

		// Cannot be itself, cannot be locked one
		if (Cast<ABaseCharacter>(HitResult.GetActor()) != BaseCharacterRef && Cast<
			ABaseCharacter>(HitResult.GetActor()) != BaseCharacterRef->LockedTarget)
		{
			HoveredTarget = Cast<ABaseCharacter>(HitResult.GetActor());

			if (HoveredTarget)
			{
				if (HoveredTarget->PlaneComponent)
				{
					if (HoveredTarget->PlaneComponent->GetStaticMesh())
					{
						HoveredTarget->PlaneComponent->SetMaterial(0, HoveredTargetMaterial);
					}
				}
			}
		}
	}
	else
	{
		// Removes current hovered target if not hovered
		if (HoveredTarget)
		{
			// only if it is not locked
			if (HoveredTarget != BaseCharacterRef->LockedTarget)
			{
				if (HoveredTarget->PlaneComponent)
				{
					if (HoveredTarget->PlaneComponent->GetStaticMesh())
					{
						HoveredTarget->PlaneComponent->SetMaterial(0, NoTargetMaterial);
						HoveredTarget = nullptr;
					}
				}
			}
		}
	}
}

void AAlzadiPlayerController::LeftMouseButtonClient()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
	else
	{
		if (IsLocalController())
		{
			if (bIsConnected)
			{
				MouseLockedClient();
			}
		}
		else
		{
		}
	}
}

void AAlzadiPlayerController::MouseLockedClient()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	// if it's locked, it has to be unlocked (for both new or old hovered target)
	if (BaseCharacterRef->LockedTarget)
	{
		// in case hovered is same that locked, it has to be unlocked
		if (BaseCharacterRef->LockedTarget == HoveredTarget)
		{
			if (BaseCharacterRef->LockedTarget->PlaneComponent)
			{
				if (BaseCharacterRef->LockedTarget->PlaneComponent->GetStaticMesh())
				{
					BaseCharacterRef->LockedTarget->PlaneComponent->SetMaterial(0, NoTargetMaterial);
					BaseCharacterRef->LockedTarget = nullptr;
				}
			}

			BaseCharacterRef->ResetLockedTarget();
		}
		// in case hovered is different than locked, old one has to be unlocked, and new one locked
		else
		{
			if (BaseCharacterRef->LockedTarget->PlaneComponent)
			{
				if (BaseCharacterRef->LockedTarget->PlaneComponent->GetStaticMesh())
				{
					BaseCharacterRef->LockedTarget->PlaneComponent->SetMaterial(0, NoTargetMaterial);
					BaseCharacterRef->LockedTarget = nullptr;
				}
			}

			BaseCharacterRef->ResetLockedTarget();

			// if there is a hovered target
			if (HoveredTarget)
			{
				BaseCharacterRef->LockedTarget = HoveredTarget;

				if (BaseCharacterRef->LockedTarget->PlaneComponent)
				{
					if (BaseCharacterRef->LockedTarget->PlaneComponent->GetStaticMesh())
					{
						BaseCharacterRef->LockedTarget->PlaneComponent->SetMaterial(0, LockedTargetMaterial);
					}
				}

				BaseCharacterRef->SetLockedTargetServer(HoveredTarget);
			}
		}
	}
	// if there is no locked target, it has to be locked
	else
	{
		// if there is a hovered target
		if (HoveredTarget)
		{
			BaseCharacterRef->LockedTarget = HoveredTarget;

			if (BaseCharacterRef->LockedTarget)
			{
				if (BaseCharacterRef->LockedTarget->PlaneComponent)
				{
					if (BaseCharacterRef->LockedTarget->PlaneComponent->GetStaticMesh())
					{
						BaseCharacterRef->LockedTarget->PlaneComponent->SetMaterial(0, LockedTargetMaterial);
					}
				}
			}

			BaseCharacterRef->SetLockedTargetServer(HoveredTarget);
		}
	}
}
