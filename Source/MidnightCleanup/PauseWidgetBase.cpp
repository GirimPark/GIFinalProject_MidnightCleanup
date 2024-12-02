// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidgetBase.h"

#include "GRNetworkSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "PC.h"
#include "InitGamePC.h"
#include "LogicHandler.h"
#include "Outgame/FriendInvitationWidgetBase.h"
#include "SettingWidgetBase.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UPauseWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnBox = Cast<UButton>(GetWidgetFromName(TEXT("ReturnBTN")));
	if (ReturnBox)
	{
		ReturnBox->OnClicked.AddDynamic(this, &UPauseWidgetBase::ReturnInGame);
	}

	FriendInviteBox = Cast<UButton>(GetWidgetFromName(TEXT("FriendInviteBTN")));
	if (FriendInviteBox)
	{
		FriendInviteBox->OnClicked.AddDynamic(this, &UPauseWidgetBase::ShowFriendInvitationWidget);
	}

	SettingBox = Cast<UButton>(GetWidgetFromName(TEXT("SettingBTN")));
	if (SettingBox)
	{
		SettingBox->OnClicked.AddDynamic(this, &UPauseWidgetBase::ShowSettingWidget);
	}

	QuitBox = Cast<UButton>(GetWidgetFromName(TEXT("QuitBTN")));
	if(QuitBox)
	{
		QuitBox->OnClicked.AddDynamic(this, &UPauseWidgetBase::OnQuitButtonClicked);
	}

	FriendInvitationWidget = Cast<UFriendInvitationWidgetBase>(GetWidgetFromName(TEXT("WBP_FriendInvitation")));
	FriendInvitationWidget->SetParentWidget(this);

	TempBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TempText")));

	SettingWidget = Cast<USettingWidgetBase>(GetWidgetFromName(TEXT("WBP_PauseSetting")));
	SettingWidget->SetParentWidget(this);

	QuitAlertCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Canvas_GameQuitAlert")));
	QuitConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_QuitConfirm")));
	QuitCancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_QuitCancel")));

	QuitConfirmButton->OnClicked.AddDynamic(this, &UPauseWidgetBase::OnQuitConfirmButtonClicked);
	QuitCancelButton->OnClicked.AddDynamic(this, &UPauseWidgetBase::OnQuitCancelButtonClicked);

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnQuitRoomSucceed.AddDynamic(this, &UPauseWidgetBase::OnQuitRoomRespond);
}

void UPauseWidgetBase::ReturnInGame()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->HidePauseWidget();
	}

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (InitGamePC)
	{
		InitGamePC->HidePauseWidget();
	}
}

void UPauseWidgetBase::ShowFriendInvitationWidget()
{
	if (FriendInvitationWidget)
	{
		ReturnBox->SetRenderOpacity(0.f);
		FriendInviteBox->SetRenderOpacity(0.f);
		SettingBox->SetRenderOpacity(0.f);
		QuitBox->SetRenderOpacity(0.f);

		FriendInvitationWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPauseWidgetBase::ShowSettingWidget()
{
	if (SettingWidget)
	{
		ReturnBox->SetRenderOpacity(0.f);
		FriendInviteBox->SetRenderOpacity(0.f);
		SettingBox->SetRenderOpacity(0.f);
		QuitBox->SetRenderOpacity(0.f);

		SettingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPauseWidgetBase::OnQuitButtonClicked()
{
	QuitAlertCanvas->SetVisibility(ESlateVisibility::Visible);
	QuitConfirmButton->SetIsEnabled(true);
	QuitCancelButton->SetIsEnabled(true);
}

void UPauseWidgetBase::OnPopupWidgetClosed()
{
	ReturnBox->SetIsEnabled(false);
	FriendInviteBox->SetIsEnabled(false);
	SettingBox->SetIsEnabled(false);
	QuitBox->SetIsEnabled(false);
	QuitConfirmButton->SetIsEnabled(false);
	QuitCancelButton->SetIsEnabled(false);

	ReturnBox->SetIsEnabled(true);
	FriendInviteBox->SetIsEnabled(true);
	SettingBox->SetIsEnabled(true);
	QuitBox->SetIsEnabled(true);
	QuitConfirmButton->SetIsEnabled(true);
	QuitCancelButton->SetIsEnabled(true);

	ReturnBox->SetRenderOpacity(1.f);
	FriendInviteBox->SetRenderOpacity(1.f);
	SettingBox->SetRenderOpacity(1.f);
	QuitBox->SetRenderOpacity(1.f);
	QuitConfirmButton->SetRenderOpacity(1.f);
	QuitCancelButton->SetRenderOpacity(1.f);
}

void UPauseWidgetBase::OnQuitRoomRespond()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelNameToQuit), true);
}

void UPauseWidgetBase::OnQuitConfirmButtonClicked()
{
	if (GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
		GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestQuitRoom();
	else
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("L_Title")), true);
}

void UPauseWidgetBase::OnQuitCancelButtonClicked()
{
	QuitAlertCanvas->SetVisibility(ESlateVisibility::Collapsed);
}

void UPauseWidgetBase::DisableFriendInviteButton()
{
	if (FriendInviteBox)
	{
		FriendInviteBox->SetIsEnabled(false);
		FriendInviteBox->SetVisibility(ESlateVisibility::Collapsed);

		if (TempBox)
		{
			TempBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPauseWidgetBase::HidePopupWidget()
{
	if (FriendInvitationWidget && SettingWidget && QuitAlertCanvas)
	{
		FriendInvitationWidget->SetVisibility(ESlateVisibility::Hidden);
		SettingWidget->SetVisibility(ESlateVisibility::Hidden);
		QuitAlertCanvas->SetVisibility(ESlateVisibility::Hidden);
	}

	OnPopupWidgetClosed();

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		DisableFriendInviteButton();
	}
}
