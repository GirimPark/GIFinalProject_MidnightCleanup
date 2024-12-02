// Fill out your copyright notice in the Description page of Project Settings.


#include "DeleteFriendAlertWidgetBase.h"

#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDeleteFriendAlertWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	DeleteAlertText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_DeleteAlert")));
	DeleteFriendButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_DeleteFriend")));
	if (DeleteFriendButton)
	{
		DeleteFriendButton->OnClicked.AddDynamic(this, &UDeleteFriendAlertWidgetBase::OnDeleteFriendButtonClicked);
	}
	ReturnButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Return")));
	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UDeleteFriendAlertWidgetBase::OnReturnButtonClicked);
	}
}

void UDeleteFriendAlertWidgetBase::OnDeleteFriendButtonClicked()
{
	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestDeleteFriend(FriendName);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDeleteFriendAlertWidgetBase::OnReturnButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
