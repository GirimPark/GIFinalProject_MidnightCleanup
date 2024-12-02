// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass)
	{
		if (IsLocalPlayerController())
		{
			LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
			LobbyWidget->AddToViewport();

			if (HasAuthority())
			{
				LobbyWidget->ShowStartButton();
			}
		}

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

bool ALobbyPC::C2S_SendMessage_Validate(const FText& Text)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(class FText const& Text)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			PC->S2C_SendMessage(Text);
		}
	}
}

void ALobbyPC::S2C_SendMessage_Implementation(class FText const& Text)
{
	if (LobbyWidget)
	{
		LobbyWidget->AddMessage(Text);
	}
}
