// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "TitleWidgetBase.h"
#include "Outgame/NicknameSettingWidgetBase.h"
#include "Outgame/NonLoginMainWidgetBase.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	if (!(NicknameSettingWidgetClass && MainWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClass is Empty"));
		return;
	}
	
	NicknameSettingWidget = CreateWidget<UNicknameSettingWidgetBase>(GetWorld(), NicknameSettingWidgetClass);
	NicknameSettingWidget->AddToViewport();

	MainWidget = CreateWidget<UNonLoginMainWidgetBase>(GetWorld(), MainWidgetClass);
	MainWidget->AddToViewport();
	MainWidget->SetVisibility(ESlateVisibility::Collapsed);


	FInputModeUIOnly InputMode;
	//InputMode.SetWidgetToFocus(NicknameSettingWidget->TakeWidget());
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);
	DefaultMouseCursor = EMouseCursor::Default;
	bShowMouseCursor = true;
}

void ATitlePC::ShowMainWidget()
{
	NicknameSettingWidget->SetVisibility(ESlateVisibility::Collapsed);
	MainWidget->SetVisibility(ESlateVisibility::Visible);
}
 
