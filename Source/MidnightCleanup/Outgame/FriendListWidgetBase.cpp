// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendListWidgetBase.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "FriendManagementWidgetBase.h"

void UFriendListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UsernameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Username")));

	FriendStateImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_FriendState")));

	FriendStateText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_FriendState")));

	DeleteFriendButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_DeleteFriend")));
	if(DeleteFriendButton)
	{
		DeleteFriendButton->OnClicked.AddDynamic(this, &UFriendListWidgetBase::OnDeleteFriendButtonClicked);
	}

}

void UFriendListWidgetBase::SetImageFromFriendState(EUserState UserState)
{
	FSlateBrush Brush = FriendStateImage->GetBrush();
	FString State;
	switch (UserState)
	{
	case EUserState::ONLINE:
	{
		Brush.TintColor = FSlateColor(OnlineStateTintColor);
		FriendStateImage->SetBrush(Brush);
		State = TEXT("온라인");
		FriendStateText->SetText(FText::FromString(State));
		FriendStateText->SetColorAndOpacity(Brush.TintColor);
		break;
	}
	case EUserState::IN_GAME:
	{
		Brush.TintColor = FSlateColor(InGameStateTintColor);
		FriendStateImage->SetBrush(Brush);
		State = TEXT("게임중");
		FriendStateText->SetText(FText::FromString(State));
		FriendStateText->SetColorAndOpacity(Brush.TintColor);
		break;
	}
	case EUserState::OFFLINE:
	{
		Brush.TintColor = FSlateColor(OfflineStateTintColor);
		FriendStateImage->SetBrush(Brush);
		State = TEXT("오프라인");
		FriendStateText->SetText(FText::FromString(State));
		FriendStateText->SetColorAndOpacity(Brush.TintColor);
		break;
	}
	default:
		UE_LOG(LogTemp, Error, TEXT("SetImageFromState() Failed: unvalid UserState"));
	}
}

void UFriendListWidgetBase::OnDeleteFriendButtonClicked()
{
	ParentWidget->ShowDeleteFriendPopup(UsernameText->GetText());
}
