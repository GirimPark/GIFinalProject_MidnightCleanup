// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveLever.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

AInteractiveLever::AInteractiveLever()
{
	bLeverActivate = false;
	bReplicates = true;
}

void AInteractiveLever::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveLever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AInteractiveLever::ActivateSequence()
{
	bLeverActivate = true;
	Mesh->SetMaterial(0, MaterialInterface);
}

void AInteractiveLever::OverlapSequence()
{
}


