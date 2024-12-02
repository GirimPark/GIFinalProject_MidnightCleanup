// Fill out your copyright notice in the Description page of Project Settings.


#include "AddFriendWidgetBase.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UAddFriendWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	NickNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_FriendName")));

	FriendStateText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_FriendState")));

	WidgetSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("WidgetSwitcher")));

	AddFriendButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_AddFriend")));

	PendingFriendButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_PendingFriend")));
}

void UAddFriendWidgetBase::SetImageFromFriendState(EUserState UserState)
{
	FSlateColor TintColor;
	FString State;
	switch (UserState)
	{
	case EUserState::ONLINE:
	{
		TintColor = FSlateColor(OnlineStateTintColor);
		State = TEXT("온라인");
		FriendStateText->SetText(FText::FromString(State));
		FriendStateText->SetColorAndOpacity(TintColor);
		break;
	}
	case EUserState::IN_GAME:
	{
		TintColor = FSlateColor(InGameStateTintColor);
		State = TEXT("게임중");
		FriendStateText->SetText(FText::FromString(State));
		FriendStateText->SetColorAndOpacity(TintColor);
		break;
	}
	case EUserState::OFFLINE:
	{
		TintColor = FSlateColor(OfflineStateTintColor);
		State = TEXT("오프라인");
		FriendStateText->SetText(FText::FromString(State));
		FriendStateText->SetColorAndOpacity(TintColor);
		break;
	}
	default:
		UE_LOG(LogTemp, Error, TEXT("SetImageFromState() Failed: unvalid UserState"));
	}
}

void UAddFriendWidgetBase::SetButtonFromRequestState(ERequestState RequestState)
{
	AddFriendButton->SetIsEnabled(true);
	switch (RequestState)
	{
	case ERequestState::NON:
		{
		WidgetSwitcher->SetActiveWidgetIndex(0);
		break;
		}
	case ERequestState::ALREADY_REQUEST:
		{
		WidgetSwitcher->SetActiveWidgetIndex(1);
		break;
		}
	case ERequestState::ALREADY_FRIEND:
		{
		WidgetSwitcher->SetActiveWidgetIndex(0);
		AddFriendButton->SetIsEnabled(false);
		break;
		}
	default:
		UE_LOG(LogTemp, Error, TEXT("SetTextureFromRequestState() Failed: unvalid UserState"));
	}
}

void UAddFriendWidgetBase::OnAddFriendRespond(bool Success)
{
	AddFriendButton->SetIsEnabled(true);
	if(Success)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UAddFriendWidgetBase::OnCancelAddFriendRespond(bool Success)
{
	if (Success)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UAddFriendWidgetBase::OnAcceptFriendRespond()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	AddFriendButton->SetIsEnabled(false);
}
