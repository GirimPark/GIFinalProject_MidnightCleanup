// Fill out your copyright notice in the Description page of Project Settings.


#include "InitGamePC.h"
#include "LaptopWidgetBase.h"
#include "ToolTipWidgetBase.h"
#include "EnhancedInputSubsystems.h"
#include "GRNetworkSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "InitGameGM.h"
#include "LogicHandler.h"
#include "PauseWidgetBase.h"
#include "Outgame/InvitationAlertWidgetBase.h"
#include "PlayerCharacter.h"
#include "LoadingWidgetBase.h"
#include "Logging/StructuredLog.h"
#include "Outgame/RoomCodeWidgetBase.h"
#include "Net/UnrealNetwork.h"

void AInitGamePC::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem && DefaultIMC)
	{
		SubSystem->AddMappingContext(DefaultIMC, 0);
	}

	if (!IsLocalPlayerController())
		return;
	
	DefaultMouseCursor = EMouseCursor::Default;
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	if(GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
	{
		if (!RoomCodeWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("RoomCodeWidgetClass isnt assigned"));
			return;
		}
		RoomCodeWidget = CreateWidget<URoomCodeWidgetBase>(this, RoomCodeWidgetClass);
		if (!RoomCodeWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("RoomCodeWidget is invalid"));
			return;
		}
		RoomCodeWidget->AddToViewport();
	}

	if (LaptopWidgetClass)
	{
		LaptopWidget = CreateWidget<ULaptopWidgetBase>(this, LaptopWidgetClass);
		if (LaptopWidget)
		{
			LaptopWidget->AddToViewport(2);
			LaptopWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (ToolTipWidgetClass)
	{
		ToolTipWidget = CreateWidget<UToolTipWidgetBase>(this, ToolTipWidgetClass);
		if (ToolTipWidget)
		{
			ToolTipWidget->AddToViewport(1);
			ShowToolTipWidget();
		}
	}

	if (AimWidgetClass)
	{
		AimWidget = CreateWidget<UUserWidget>(this, AimWidgetClass);
		if (AimWidget)
		{
			AimWidget->AddToViewport();
			AimWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UPauseWidgetBase>(this, PauseWidgetClass);
		if (PauseWidget)
		{
			PauseWidget->AddToViewport();
			PauseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (InvitationAlertWidgetClass)
	{
		InvitationAlertWidget = CreateWidget<UInvitationAlertWidgetBase>(this,InvitationAlertWidgetClass);
		if(InvitationAlertWidget)
		{
			InvitationAlertWidget->AddToViewport();
			InvitationAlertWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	auto& OnJoinRoomRespond = GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnJoinRoomRespond;
	OnJoinRoomRespond.Clear();
	OnJoinRoomRespond.AddDynamic(this, &AInitGamePC::OnJoinRoomRespond);
}

void AInitGamePC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInitGamePC, OpenLevelName);
}

void AInitGamePC::ShowLaptopWidget()
{
	HideToolTipWidget();
	AimWidget->SetVisibility(ESlateVisibility::Hidden);

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetWidgetToFocus(LaptopWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);

	if (LaptopWidget)
	{
		LaptopWidget->SetVisibility(ESlateVisibility::Visible);
		LaptopWidget->PlayInitSound();
	}
}

void AInitGamePC::HideLaptopWidget()
{
	ShowToolTipWidget();
	AimWidget->SetVisibility(ESlateVisibility::Visible);

	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	if (LaptopWidget)
	{
		LaptopWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	APlayerCharacter* OwnedPlayer = Cast<APlayerCharacter>(GetCharacter());
	if (OwnedPlayer)
	{
		OwnedPlayer->DisableLaptop();
	}
}

void AInitGamePC::ShowToolTipWidget()
{
	if (ToolTipWidget)
	{
		ToolTipWidget->SetRenderOpacity(1.f);
	}
}

void AInitGamePC::HideToolTipWidget()
{
	if (ToolTipWidget)
	{
		ToolTipWidget->SetRenderOpacity(0.f);
	}
}

void AInitGamePC::UpdateToolTip(FText InText)
{
	if (ToolTipWidget)
	{
		ToolTipWidget->UpdateScript(InText);
	}
}

void AInitGamePC::ShowPauseWidget()
{
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Visible);

		if (!GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
			PauseWidget->DisableFriendInviteButton();

		bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		SetInputMode(InputMode);

		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);
	}
}

void AInitGamePC::HidePauseWidget()
{
	if (PauseWidget)
	{
		HideToolTipWidget();

		PauseWidget->HidePopupWidget();

		PauseWidget->SetVisibility(ESlateVisibility::Hidden);

		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}

bool AInitGamePC::IsEnablePauseWidget()
{
	if (PauseWidget && PauseWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AInitGamePC::IsEnableLaptopWidget()
{
	if (LaptopWidget && LaptopWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AInitGamePC::ShowLoadingWidget()
{
	if (!IsLocalController())
		return;

	bShowMouseCursor = false;
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	const FString WidgetPath = TEXT("/Game/UI/WBP_Loading.WBP_Loading_C");
	TSubclassOf<ULoadingWidgetBase> LoadingWidgetClass = StaticLoadClass(ULoadingWidgetBase::StaticClass(), nullptr, *WidgetPath);

	if (LoadingWidgetClass)
	{
		ULoadingWidgetBase* LoadingWidget = CreateWidget<ULoadingWidgetBase>(this, LoadingWidgetClass);

		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport(10);
			LoadingWidget->PlayAnim();
			LoadingWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AInitGamePC::SetOpenLevelName(const FString& InName)
{
	C2S_SetOpenLevelName(InName);
}

void AInitGamePC::OpenInGame()
{
	C2S_OpenInGame();
}

void AInitGamePC::EnableLoadingScreen()
{
	APlayerCharacter* OwnedPlayer = Cast<APlayerCharacter>(GetPawn());
	if (OwnedPlayer)
	{
		C2S_EnableLoadingScreen(OwnedPlayer);
	}
}

void AInitGamePC::C2S_EnableLoadingScreen_Implementation(APlayerCharacter* InPlayer)
{
	InPlayer->S2A_EnableLoadingScreen();
}

void AInitGamePC::C2S_OpenInGame_Implementation()
{
	AInitGameGM* GM = Cast<AInitGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	//UE_LOG(LogTemp, Error, TEXT("AInitGamePC::C2S_OpenInGame_Implementation() : %s"), *OpenLevelName);
	if (GM)
	{
		GM->OpenInGame(OpenLevelName);
	}
}

void AInitGamePC::C2S_SetOpenLevelName_Implementation(const FString& InLevelName)
{
	S2A_SetOpenLevelName(InLevelName);
}

void AInitGamePC::S2A_SetOpenLevelName_Implementation(const FString& InLevelName)
{
	OpenLevelName = InLevelName;
}

void AInitGamePC::OnJoinRoomRespond(int32 ResultCode, FString IpAddress)
{
	if (ResultCode != EJoinRoomResponse::SUCCESS)
		return;

	UGameplayStatics::OpenLevel(GetWorld(), FName(*IpAddress), true);
}


