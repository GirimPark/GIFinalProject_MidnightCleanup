// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidgetBase.h"

#include "PauseWidgetBase.h"
#include "Components/Button.h"

void USettingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnButton = Cast<UButton>(GetWidgetFromName(TEXT("ReturnBTN")));
	ReturnButton->OnClicked.AddDynamic(this, &USettingWidgetBase::OnReturnButtonClicked);
}

void USettingWidgetBase::OnReturnButtonClicked()
{
	if(ParentWidget)
		ParentWidget->OnPopupWidgetClosed();
	SetVisibility(ESlateVisibility::Collapsed);
}
