// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendInvitationWidgetBase.h"

#include "ActivePlayer.h"
#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "Components/ScrollBox.h"
#include "PauseFriendListWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MidnightCleanup/PauseWidgetBase.h"

void UFriendInvitationWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	FriendListScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_FriendList")));

	ReturnButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Return")));
	if(ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UFriendInvitationWidgetBase::OnReturnButtonClicked);
	}

	OnVisibilityChanged.AddDynamic(this, &UFriendInvitationWidgetBase::OnWidgetVisible);

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnInviteFriendRespond.AddDynamic(this, &UFriendInvitationWidgetBase::OnInviteFriendRespond);
}

void UFriendInvitationWidgetBase::OnReturnButtonClicked()
{
	ParentWidget->OnPopupWidgetClosed();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UFriendInvitationWidgetBase::OnWidgetVisible(ESlateVisibility NewVisibility)
{
	if(NewVisibility == ESlateVisibility::Visible)
	{
		SetFriendList(GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->GetActivePlayerInfoRef()->FriendList);
	}
}

void UFriendInvitationWidgetBase::OnInviteFriendRespond(bool Success, FString InvitedUserName)
{
	if (!Success)
		return;

	TArray<UWidget*> FriendWidgetList = FriendListScrollBox->GetAllChildren();
	UPauseFriendListWidgetBase* TargetFriendWidget = nullptr;
	for(auto& Widget : FriendWidgetList)
	{
		UPauseFriendListWidgetBase* FriendWidget = Cast<UPauseFriendListWidgetBase>(Widget);
		if (!FriendWidget)
			continue;

		if(FriendWidget->NickNameText->GetText().ToString() == InvitedUserName)
		{
			FriendWidget->InviteFriendButton->SetIsEnabled(false);

			TargetFriendWidget = FriendWidget;
			break;
		}
	}

	if (!TargetFriendWidget)
		return;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[TargetFriendWidget]()
		{
			TargetFriendWidget->InviteFriendButton->SetIsEnabled(true);
		}
		, DelayTime, false);
}

void UFriendInvitationWidgetBase::SetFriendList(TArray<FFriendInfo> FriendList)
{
	SortFriendInfoArray(FriendList);

	if (!FriendListScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("FriendListScrollBox is nullptr"));
		return;
	}
	if (!FriendListWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("FriendListWidgetClass is unvalid"));
		return;
	}

	FriendListScrollBox->ClearChildren();

	for (const auto& Info : FriendList)
	{
		UPauseFriendListWidgetBase* NewItem = CreateWidget<UPauseFriendListWidgetBase>(GetWorld(), FriendListWidgetClass);
		if (NewItem)
		{
			FriendListScrollBox->AddChild(NewItem);

			// Set User Info
			NewItem->NickNameText.Get()->SetText(FText::FromString(Info.Name));
			NewItem->SetFriendStateImage(Info.State);
		}
	}
}

void UFriendInvitationWidgetBase::SortFriendInfoArray(TArray<FFriendInfo>& FriendInfoArray)
{
	FriendInfoArray.Sort([](const FFriendInfo& A, const FFriendInfo& B)
		{
			return A.Name < B.Name;
		});

	FriendInfoArray.Sort([](const FFriendInfo& A, const FFriendInfo& B)
		{
			return A.State > B.State;
		});
}

