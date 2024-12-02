// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "TitleGM.h"
#include "Kismet/GameplayStatics.h"
#include "TitlePC.h"
#include "GI.h"

void UTitleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Base
	GameStartBox = Cast<UButton>(GetWidgetFromName("GameStartButton"));
	if (GameStartBox)
	{
		GameStartBox->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnGameStartButton);
	}

	SettingBox = Cast<UButton>(GetWidgetFromName("SettingButton"));
	if (SettingBox)
	{
		SettingBox->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnSettingButton);
	}

	ExitBox = Cast<UButton>(GetWidgetFromName("ExitButton"));
	if (ExitBox)
	{
		ExitBox->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnExitButton);
	}

	// Game Start
	GameStartPanel = Cast<UCanvasPanel>(GetWidgetFromName("GameStart"));
	
	InputAddressPanel = Cast<UCanvasPanel>(GetWidgetFromName("InputAddress"));

	InputNameBox = Cast<UEditableTextBox>(GetWidgetFromName("NameInput"));
	if (InputNameBox)
	{
		InputNameBox->OnTextChanged.AddDynamic(this, &UTitleWidgetBase::OnCommitName);
		InputNameBox->OnTextCommitted.AddDynamic(this, &UTitleWidgetBase::OnEnterName);
		UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted"));
	}	

	InputAddressBox = Cast<UEditableTextBox>(GetWidgetFromName("AddressInput"));
	if (InputAddressBox)
	{
		InputAddressBox->OnTextChanged.AddDynamic(this, &UTitleWidgetBase::OnCommitAddress);
		InputAddressBox->OnTextCommitted.AddDynamic(this, &UTitleWidgetBase::OnEnterAddress);
		UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted"));
	}

	ServerBox = Cast<UButton>(GetWidgetFromName("ServerButton"));
	if (ServerBox)
	{
		ServerBox->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnServerButton);
	}

	ConnectBox = Cast<UButton>(GetWidgetFromName("ConnectButton"));
	if (ConnectBox)
	{
		ConnectBox->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnConnectButton);
	}

	NextBox = Cast<UButton>(GetWidgetFromName("NextButton"));
	if (NextBox)
	{
		NextBox->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnNextButton);
	}
}

void UTitleWidgetBase::OnGameStartButton()
{
	if (InputAddressPanel)
	{
		InputAddressPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTitleWidgetBase::OnSettingButton()
{
}

void UTitleWidgetBase::OnExitButton()
{
}

void UTitleWidgetBase::OnCommitName(const FText& Text)
{	
	UE_LOG(LogTemp, Warning, TEXT("OnCommitName"));
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCommitName22"));
		GI->SetUserID(Text.ToString());
	}		
}

void UTitleWidgetBase::OnEnterName(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		InputNameBox->SetText(FText::FromString(""));
	}
	break;
	case ETextCommit::OnCleared:
	{
		InputNameBox->SetUserFocus(GetOwningPlayer());
	}
	break;
	}
}

void UTitleWidgetBase::OnCommitAddress(const FText& Text)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCommitAddress"));

	IpAddress = FName(Text.ToString());	
}

void UTitleWidgetBase::OnEnterAddress(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch(CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			InputAddressBox->SetText(FText::FromString(""));
		}
		break;
		case ETextCommit::OnCleared:
		{
			InputAddressBox->SetUserFocus(GetOwningPlayer());
		}
		break;
	}
}

void UTitleWidgetBase::OnServerButton()
{
	ATitleGM* GM = Cast<ATitleGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		UGameplayStatics::OpenLevel(this, FName(*GM->InitLevelName), true, "listen");
	}
}

void UTitleWidgetBase::OnConnectButton()
{
	UGameplayStatics::OpenLevel(this, IpAddress, true);
}

void UTitleWidgetBase::OnNextButton()
{
	if (InputAddressPanel)
	{
		InputAddressPanel->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameStartPanel)
	{
		GameStartPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTitleWidgetBase::SaveUserID()
{
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		if (InputNameBox)
		{
			GI->SetUserID(InputNameBox->GetText().ToString());
		}
	}
}

void UTitleWidgetBase::ShowServerButton()
{
	ServerBox->SetVisibility(ESlateVisibility::Visible);
}

void UTitleWidgetBase::HideServerButton()
{
	ServerBox->SetVisibility(ESlateVisibility::Hidden);
}
