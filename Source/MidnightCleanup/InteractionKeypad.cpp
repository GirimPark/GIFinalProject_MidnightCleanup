// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionKeypad.h"
#include "Kismet/GameplayStatics.h"
#include "InitGamePC.h"
#include "PlayerCharacter.h"
#include "Seunggi/InGameGM.h"
#include "Seunggi/InGameGS.h"
#include "PC.h"

void AInteractionKeypad::InterAction(APawn* Character)
{
	APC* PC = Cast<APC>(Character->GetController());
	if (!PC)
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		GetWorld()->GetAuthGameMode<AInGameGM>()->FinishGame();
	}
}

void AInteractionKeypad::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionKeypad::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = nullptr;
		AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			ToolTip = GS->GetToolTipByID(FName(TEXT("WorkStart")));
		}
		else
		{
			ToolTip = GS->GetToolTipByID(FName(TEXT("WorkFinish")));
		}

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
}

void AInteractionKeypad::S2A_ShowRewardWidget_Implementation(APlayerCharacter* Player)
{
	APC* PC= Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		if(PC->bIsInTruck)
		{
			UE_LOG(LogTemp, Warning, TEXT("In Truck"));
			PC->ShowRewardWidget(PC->GetPawn<APlayerCharacter>());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Out Truck"));
		}
	}
}
