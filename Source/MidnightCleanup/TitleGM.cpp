// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGM.h"

void ATitleGM::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}