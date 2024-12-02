// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetBase.h"

#include "FriendManagementWidgetBase.h"
#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "OutGamePC.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MidnightCleanup/SettingWidgetBase.h"
#include "CreditWidgetBase.h"

void UMainWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	FriendManagementButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_ManageFriend")));
	SettingButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Setting")));
	SettingWidget = Cast<USettingWidgetBase>(GetWidgetFromName(TEXT("WBP_PauseSetting")));
	FriendAlertImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_FriendAlert")));
	RoomCodeText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("PopupText_RoomCode")));
	JoinRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("PopupButton_JoinRoom")));
	JoinRoomText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_JoinRoom")));
	JoinRoomResText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_JoinRoomRes")));
	CreditButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Credit")));

	FriendManagementButton->OnClicked.AddDynamic(this, &UMainWidgetBase::OnFriendManagementButtonClicked);
	SettingButton->OnClicked.AddDynamic(this, &UMainWidgetBase::OnSettingButtonClicked);
	RoomCodeText->OnTextCommitted.AddDynamic(this, &UMainWidgetBase::OnTextCommitted);
	CreditButton->OnClicked.AddDynamic(this, &UMainWidgetBase::OnCreditButtonClicked);

	auto LogicHandler = GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler();
	LogicHandler->OnJoinRoomRespond.AddDynamic(this, &UMainWidgetBase::OnJoinRoomRespond);
	LogicHandler->OnAddFriendNotified.AddDynamic(this, &UMainWidgetBase::SetVisibleFriendAlert);
	LogicHandler->OnAcceptFriendNotified.AddDynamic(this, &UMainWidgetBase::SetVisibleFriendAlert);
	LogicHandler->OnCancelAddFriendNotified.AddDynamic(this, &UMainWidgetBase::SetCollapsedFriendAlert);
}

void UMainWidgetBase::OnRoomCreated()
{
	UGameplayStatics::OpenLevel(this, FName(*CreatedRoomLevelName), true, "listen");
}

void UMainWidgetBase::OnRoomJoined(FString IpAddress)
{
	UGameplayStatics::OpenLevel(this, FName(*IpAddress), true);
	UE_LOG(LogTemp, Warning, TEXT("%s"), TCHAR_TO_UTF8(*IpAddress));
}

void UMainWidgetBase::OnFriendManagementButtonClicked()
{
	FriendAlertImage->SetVisibility(ESlateVisibility::Collapsed);

	UFriendManagementWidgetBase* FriendManagementWidget = Cast<AOutGamePC>(GetOwningPlayer())->FriendManagementWidget;
	FriendManagementWidget->GetFriendListButton()->OnClicked.Broadcast();
}

void UMainWidgetBase::OnSettingButtonClicked()
{
	SettingWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidgetBase::OnJoinRoomRespond(int32 ResultCode, FString IpAddress)
{
	switch (ResultCode)
	{
	case EJoinRoomResponse::SUCCESS:
	{
		JoinRoomResText->SetVisibility(ESlateVisibility::Collapsed);
		JoinRoomButton->SetIsEnabled(false);
		JoinRoomText->SetText(FText::FromString(TEXT("접속중")));
		UGameplayStatics::OpenLevel(GetWorld(), FName(*IpAddress), true);
		break;
	}
	case EJoinRoomResponse::OVER_PLAYER:
	{
		JoinRoomResText->SetVisibility(ESlateVisibility::Visible);
		JoinRoomResText->SetText(FText::FromString(TEXT("모집이 완료된 팀입니다.")));
		break;
	}
	case EJoinRoomResponse::INVALID_ROOM:
	{
		JoinRoomResText->SetVisibility(ESlateVisibility::Visible);
		JoinRoomResText->SetText(FText::FromString(TEXT("유효하지 않은 팀 코드입니다.")));
		break;
	}
	}
}

void UMainWidgetBase::SetVisibleFriendAlert()
{
	FriendAlertImage->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidgetBase::SetCollapsedFriendAlert()
{
	FriendAlertImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if(CommitMethod == ETextCommit::Type::OnEnter)
	{
		JoinRoomButton->OnClicked.Broadcast();
	}
}

void UMainWidgetBase::OnCreditButtonClicked()
{
	AOutGamePC* PC = Cast<AOutGamePC>(GetOwningPlayer());
	PC->bShowMouseCursor = false;
	PC->CreditWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
}
