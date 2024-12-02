// Fill out your copyright notice in the Description page of Project Settings.


#include "NicknameSettingWidgetBase.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "MidnightCleanup/TitlePC.h"
#include "MidnightCleanup/GI.h"

void UNicknameSettingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	NicknameText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Text_Nickname")));
	EnterGameButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_EnterGame")));


	if (!(NicknameText && EnterGameButton))
	{
		UE_LOG(LogTemp, Error, TEXT("Widget is Invalid"));
		return;
	}

	EnterGameButton->OnClicked.AddDynamic(this, &UNicknameSettingWidgetBase::OnEnterGameButtonClicked);
	NicknameText->OnTextCommitted.AddDynamic(this, &UNicknameSettingWidgetBase::OnNicknameCommitted);
}

void UNicknameSettingWidgetBase::OnEnterGameButtonClicked()
{
	UGI* GI = Cast<UGI>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to UGI Failed(UNicknameSettingWidgetBase::OnEnterGameButtonClicked)"));
		return;
	}
	GI->SetUserID(NicknameText->GetText().ToString());

	NicknameText->SetText(FText::FromString(TEXT("")));

	ATitlePC* PC = Cast<ATitlePC>(GetOwningPlayer());
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to ATitlePC Failed(UNicknameSettingWidgetBase::OnEnterGameButtonClicked)"));
		return;
	}
	PC->ShowMainWidget();
}

void UNicknameSettingWidgetBase::OnNicknameCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	if(CommitType == ETextCommit::Type::OnEnter)
		EnterGameButton->OnClicked.Broadcast();
}
