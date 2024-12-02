// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGM.h"
#include "LobbyPC.h"	
#include "GI.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	StartBox = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	if (StartBox)
	{
		StartBox->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnStartButton);
	}

	InputChatBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatInput")));
	if (InputChatBox)
	{
		InputChatBox->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnCommitChat);
		InputChatBox->SetKeyboardFocus();
	}

	ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScrollBox")));
}

void ULobbyWidgetBase::OnStartButton()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidgetBase::OnCommitChat(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			ALobbyPC* PC = GetOwningPlayer<ALobbyPC>();
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (PC && GI)
			{
				FString Temp = FString::Printf(TEXT("[%s] : %s"), *GI->GetUserID(), *Text.ToString());
				PC->C2S_SendMessage(FText::FromString(Temp));
				InputChatBox->SetText(FText::FromString(TEXT("")));
			}
		}
		break;

		case ETextCommit::OnCleared:
		{
			InputChatBox->SetUserFocus(GetOwningPlayer());
		}
		break;
	}
}

void ULobbyWidgetBase::ShowStartButton()
{
	if (StartBox)
	{
		StartBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidgetBase::AddMessage(const FText& Text)
{
	TObjectPtr<UTextBlock> NewMessage = NewObject<UTextBlock>(ChatBox);
	if (NewMessage)
	{
		NewMessage->SetText(Text);
		FSlateFontInfo FontInfo = NewMessage->GetFont();
		FontInfo.Size = 18;
		NewMessage->SetFont(FontInfo);

		ChatBox->AddChild(NewMessage);
		ChatBox->ScrollToEnd();
	}
}
