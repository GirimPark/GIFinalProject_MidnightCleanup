// Fill out your copyright notice in the Description page of Project Settings.


#include "InvitationAlertWidgetBase.h"

#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MidnightCleanup/GI.h"
#include "MidnightCleanup/InitGamePC.h"

void UInvitationAlertWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	InvitationMessageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Message")));
	AcceptButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Accept")));
	RefuseButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Refuse")));

	AcceptButton->OnClicked.Clear();
	RefuseButton->OnClicked.Clear();
	AcceptButton->OnClicked.AddDynamic(this, &UInvitationAlertWidgetBase::OnAcceptButtonClicked);
	RefuseButton->OnClicked.AddDynamic(this, &UInvitationAlertWidgetBase::OnRefuseButtonClicked);

	auto& OnInviteFriendNotified = GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnInviteFriendNotified;
	OnInviteFriendNotified.Clear();
	OnInviteFriendNotified.AddDynamic(this, &UInvitationAlertWidgetBase::OnInvitationNotified);

	OnVisibilityChanged.Clear();
	OnVisibilityChanged.AddDynamic(this, &UInvitationAlertWidgetBase::OnPopupVisibilityChanged);
}

void UInvitationAlertWidgetBase::OnAcceptButtonClicked()
{
	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestJoinRoom(InvitedRoomCode);

	InvitedRoomCode.Reset();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInvitationAlertWidgetBase::OnRefuseButtonClicked()
{
	InvitedRoomCode.Reset();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInvitationAlertWidgetBase::OnInvitationNotified(FString FriendName, FString RoomCode)
{
	SetVisibility(ESlateVisibility::Visible);
	FString Message = FString::Printf(TEXT("%s 님이 팀으로 초대했습니다."), *FriendName);
	InvitationMessageText->SetText(FText::FromString(Message));

	InvitedRoomCode = RoomCode;
	UGI* GameInstance = Cast<UGI>(GetGameInstance());
	GameInstance->SetRoomCode(RoomCode);
}

void UInvitationAlertWidgetBase::OnPopupVisibilityChanged(ESlateVisibility NewVisibility)
{
	APlayerController* PC = GetOwningPlayer();

	if (NewVisibility == ESlateVisibility::Visible)
	{
		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;

		return;
	}

	if(Cast<AInitGamePC>(PC))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}
