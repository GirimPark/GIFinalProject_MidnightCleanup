// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"

AAxe::AAxe()
{
	SetName(TEXT("Axe"));
	SetToolType(EToolType::ToolType_Axe);
}

void AAxe::S2A_EnableEffect_Implementation()
{
	EnableEffect();
}
