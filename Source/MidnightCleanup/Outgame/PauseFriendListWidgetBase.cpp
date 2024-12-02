// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseFriendListWidgetBase.h"

#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPauseFriendListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	NickNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_NickName")));

	OnlineStateImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_OnlineState")));

	OfflineStateImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_OfflineState")));

	IngameStateImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_IngameState")));

	InviteFriendButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_InviteFriend")));

	if(InviteFriendButton)
	{
		InviteFriendButton->OnClicked.AddDynamic(this, &UPauseFriendListWidgetBase::RequestInviteFriend);
	}
}

void UPauseFriendListWidgetBase::RequestInviteFriend()
{
	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestInviteFriend(NickNameText->GetText().ToString());
}

void UPauseFriendListWidgetBase::SetFriendStateImage(EUserState State)
{
	OnlineStateImage->SetVisibility(ESlateVisibility::Collapsed);
	OfflineStateImage->SetVisibility(ESlateVisibility::Collapsed);
	IngameStateImage->SetVisibility(ESlateVisibility::Collapsed);
	InviteFriendButton->SetIsEnabled(true);

	switch (State)
	{
	case EUserState::ONLINE:
		{
		OnlineStateImage->SetVisibility(ESlateVisibility::Visible);
		break;
		}
	case EUserState::OFFLINE:
	{
		OfflineStateImage->SetVisibility(ESlateVisibility::Visible);
		InviteFriendButton->SetIsEnabled(false);
		break;
	}
	case EUserState::IN_GAME:
	{
		IngameStateImage->SetVisibility(ESlateVisibility::Visible);
		InviteFriendButton->SetIsEnabled(false);
		break;
	}
	default:
		UE_LOG(LogTemp, Error, TEXT("UPauseFriendListWidgetBase::SetFriendStateImage, Invalid User State"));
	}
}