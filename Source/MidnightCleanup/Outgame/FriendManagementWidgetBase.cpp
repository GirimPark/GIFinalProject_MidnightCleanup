// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendManagementWidgetBase.h"

#include "ActivePlayer.h"
#include "AddFriendWidgetBase.h"
#include "DeleteFriendAlertWidgetBase.h"
#include "FriendListWidgetBase.h"
#include "GRNetworkSubsystem.h"
#include "LogicHandler.h"
#include "PendingListWidgetBase.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UFriendManagementWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	AddFriendCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Canvas_AddFriend")));
	FriendListScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_FriendList")));
	PendingListScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_PendingList")));
	MyNicknameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_MyNickname")));
	NoFriendText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_NoFriend")));
	NoPendingText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_NoPending")));
	SearchingFriendNameText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_FriendName")));
	FindFriendFailedText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_SearchResult")));
	FriendListButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_FriendList")));
	PendingListButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_PendingList")));
	SearchFriendButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_SearchFriend")));
	FriendAlertImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_FriendAlert")));
	AddFriendWidget = Cast<UAddFriendWidgetBase>(GetWidgetFromName(TEXT("WBP_AddFriend")));
	DeleteFriendWidget = Cast<UDeleteFriendAlertWidgetBase>(GetWidgetFromName(TEXT("WBP_DeleteFriendAlert")));

	PendingListButton->OnClicked.AddDynamic(this, &UFriendManagementWidgetBase::OnPendingListButtonClicked);
	SearchingFriendNameText->OnTextCommitted.AddDynamic(this, &UFriendManagementWidgetBase::OnTextCommitted);

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnAddFriendNotified.AddDynamic(this, &UFriendManagementWidgetBase::SetVisibleFriendAlert);
}

void UFriendManagementWidgetBase::SetFriendList(TArray<FFriendInfo> FriendList)
{
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

	NoFriendText->SetVisibility(ESlateVisibility::Collapsed);
	if (FriendList.IsEmpty())
	{
		NoFriendText->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	SortFriendInfoArray(FriendList);
	for (const auto& Info : FriendList)
	{
		UFriendListWidgetBase* NewItem = CreateWidget<UFriendListWidgetBase>(GetWorld(), FriendListWidgetClass);
		if (NewItem)
		{
			NewItem->SetParentWidget(this);
			FriendListScrollBox->AddChild(NewItem);

			// Set User Info
			NewItem->UsernameText->SetText(FText::FromString(Info.Name));
			NewItem->SetImageFromFriendState(Info.State);
		}
	}
}

void UFriendManagementWidgetBase::SetPendingList(TArray<FFriendInfo> PendingList)
{
	if (!PendingListScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("PendingListScrollBox is nullptr"));
		return;
	}
	if (!PendingListWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PendingListWidgetClass is unvalid"));
		return;
	}

	PendingListScrollBox->ClearChildren();

	NoPendingText->SetVisibility(ESlateVisibility::Collapsed);
	if (PendingList.IsEmpty())
	{
		NoPendingText->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	SortFriendInfoArray(PendingList);
	for (const auto& Info : PendingList)
	{
		UPendingListWidgetBase* NewItem = CreateWidget<UPendingListWidgetBase>(GetWorld(), PendingListWidgetClass);

		if (NewItem)
		{
			PendingListScrollBox->AddChild(NewItem);

			// Set User Info
			NewItem->UsernameText.Get()->SetText(FText::FromString(Info.Name));
		}
	}
}

void UFriendManagementWidgetBase::OnFriendFound(bool Exist, FFriendInfo FriendInfo, int32 RequestState)
{
	FindFriendFailedText->SetVisibility(ESlateVisibility::Collapsed);
	AddFriendWidget->SetVisibility(ESlateVisibility::Collapsed);

	if(!Exist)
	{
		FindFriendFailedText->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	AddFriendWidget->SetVisibility(ESlateVisibility::Visible);
	AddFriendWidget->NickNameText->SetText(FText::FromString(FriendInfo.Name));
	
	AddFriendWidget->SetImageFromFriendState(FriendInfo.State);
	AddFriendWidget->SetButtonFromRequestState(static_cast<ERequestState>(RequestState));
}

void UFriendManagementWidgetBase::OnPendingListButtonClicked()
{
	FriendAlertImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UFriendManagementWidgetBase::SetVisibleFriendAlert()
{
	FriendAlertImage->SetVisibility(ESlateVisibility::Visible);
}

void UFriendManagementWidgetBase::OnSearchActionTriggered()
{
	if (AddFriendCanvas->GetVisibility() == ESlateVisibility::Visible)
		SearchFriendButton->OnClicked.Broadcast();
}

void UFriendManagementWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		SearchFriendButton->OnClicked.Broadcast();
	}
}

void UFriendManagementWidgetBase::ShowDeleteFriendPopup(FText FriendName)
{
	DeleteFriendWidget->SetVisibility(ESlateVisibility::Visible);
	DeleteFriendWidget->SetFriendName(FriendName.ToString());
	FString Message = FString::Printf(TEXT("%s 님을 친구 해제 하시겠습니까?"), *FriendName.ToString());
	DeleteFriendWidget->DeleteAlertText->SetText(FText::FromString(Message));
}

void UFriendManagementWidgetBase::SortFriendInfoArray(TArray<FFriendInfo>& FriendInfoArray)
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
