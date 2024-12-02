// Fill out your copyright notice in the Description page of Project Settings.


#include "NonLoginMainWidgetBase.h"
#include "MidnightCleanup/TitleGM.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "MidnightCleanup/SettingWidgetBase.h"

void UNonLoginMainWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	CreateRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_CreateRoom")));
	JoinRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_JoinRoom")));
	SettingButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Setting")));
	JoinRoomCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Canvas_JoinRoom")));
	JoinRoomCheckButton = Cast<UButton>(GetWidgetFromName(TEXT("PopupButton_JoinRoom")));
	IpAddressText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_IpAddress")));
	SettingWidget = Cast<USettingWidgetBase>(GetWidgetFromName(TEXT("WBP_PauseSetting")));

	CreateRoomButton->OnClicked.AddDynamic(this, &UNonLoginMainWidgetBase::OnCreateRoomButtonClicked);
	JoinRoomButton->OnClicked.AddDynamic(this, &UNonLoginMainWidgetBase::OnJoinRoomButtonClicked);
	SettingButton->OnClicked.AddDynamic(this, &UNonLoginMainWidgetBase::OnSettingButtonClicked);
	JoinRoomCheckButton->OnClicked.AddDynamic(this, &UNonLoginMainWidgetBase::OnJoinRoomCheckButtonClicked);
	IpAddressText->OnTextCommitted.AddDynamic(this, &UNonLoginMainWidgetBase::OnIpAddressTextCommitted);
}

void UNonLoginMainWidgetBase::OnCreateRoomButtonClicked()
{
	ATitleGM* GM = Cast<ATitleGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to TitleGM Failed"));
		return;
	}

	UGameplayStatics::OpenLevel(this, FName(*GM->InitLevelName), true, "listen");
}

void UNonLoginMainWidgetBase::OnJoinRoomButtonClicked()
{
	JoinRoomCanvas->SetVisibility(ESlateVisibility::Visible);
	IpAddressText->SetText(FText::FromString(TEXT("")));
}

void UNonLoginMainWidgetBase::OnSettingButtonClicked()
{
	SettingWidget->SetVisibility(ESlateVisibility::Visible);
}

void UNonLoginMainWidgetBase::OnJoinRoomCheckButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName(*IpAddressText->GetText().ToString()), true);
}

void UNonLoginMainWidgetBase::OnIpAddressTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	JoinRoomCheckButton->OnClicked.Broadcast();
}
