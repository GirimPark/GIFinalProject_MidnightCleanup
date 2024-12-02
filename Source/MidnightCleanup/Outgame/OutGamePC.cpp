// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGamePC.h"

#include "Blueprint/UserWidget.h"
#include "MainWidgetBase.h"
#include "LoginWidgetBase.h"
#include "FriendManagementWidgetBase.h"
#include "GRNetworkSubsystem.h"
#include "InvitationAlertWidgetBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CreditWidgetBase.h"
#include "MidnightCleanup/GI.h"

void AOutGamePC::BeginPlay()
{
	Super::BeginPlay();

	if(!(LoginWidgetClass && MainWidgetClass && FriendManagementWidgetClass && InvitationAlertWidgetClass && CreditWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("OutGamePC : Some WidgetClasses aren't assigned"));
		return;
	}

	LoginWidget = CreateWidget<ULoginWidgetBase>(GetWorld(), LoginWidgetClass);
	LoginWidget->AddToViewport();
	LoginWidget->SetVisibility(ESlateVisibility::Collapsed);

	MainWidget = CreateWidget<UMainWidgetBase>(GetWorld(), MainWidgetClass);
	MainWidget->AddToViewport();
	MainWidget->SetVisibility(ESlateVisibility::Collapsed);

	FriendManagementWidget = CreateWidget<UFriendManagementWidgetBase>(GetWorld(), FriendManagementWidgetClass);
	FriendManagementWidget->AddToViewport();
	FriendManagementWidget->SetVisibility(ESlateVisibility::Collapsed);

	InvitationAlertWidget = CreateWidget<UInvitationAlertWidgetBase>(GetWorld(), InvitationAlertWidgetClass);
	InvitationAlertWidget->AddToViewport();
	InvitationAlertWidget->SetVisibility(ESlateVisibility::Collapsed);

	CreditWidget = CreateWidget<UCreditWidgetBase>(GetWorld(), CreditWidgetClass);
	CreditWidget->AddToViewport();
	CreditWidget->SetVisibility(ESlateVisibility::Collapsed);


	if(GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
	{
		MainWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LoginWidget->SetVisibility(ESlateVisibility::Visible);
	}


	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem && DefaultIMC)
	{
		SubSystem->AddMappingContext(DefaultIMC, 0);

		UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(InputComponent);
		if (UIC)
		{
			UIC->BindAction(LoginAction, ETriggerEvent::Triggered,LoginWidget.Get(), &ULoginWidgetBase::OnLoginActionTriggered);
			UIC->BindAction(SearchAction, ETriggerEvent::Triggered,FriendManagementWidget.Get(), &UFriendManagementWidgetBase::OnSearchActionTriggered);
			UIC->BindAction(QuitCreditAction, ETriggerEvent::Triggered, this, &AOutGamePC::OnQuitCreditActionTriggered);
		}
	}

	DefaultMouseCursor = EMouseCursor::Default;
	bShowMouseCursor = true;
}

void AOutGamePC::OnQuitCreditActionTriggered()
{
	if (CreditWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		CreditWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
	}
}


