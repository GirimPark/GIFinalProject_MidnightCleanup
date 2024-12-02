// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"

void ALobbyGM::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ALobbyGM::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ALobbyGM::StartGame()
{
	GetWorld()->ServerTravel(LevelName);
}

