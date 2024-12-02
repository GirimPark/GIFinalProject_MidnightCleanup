// Fill out your copyright notice in the Description page of Project Settings.


#include "LimitTimeMonitor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Seunggi/InGameGS.h"
#include "LimitTimeBase.h"
#include "PC.h"

void ALimitTimeMonitor::BeginPlay()
{
	Super::BeginPlay();

	if (Widget)
	{
		if (Widget->GetWidgetClass())
		{
			UUserWidget* LimitTimeMonitor = CreateWidget<UUserWidget>(GetWorld(), Widget->GetWidgetClass());
			Widget->SetWidget(LimitTimeMonitor);
			Widget->InitWidget();

			if (LimitTimeMonitor)
			{
				LimitTimeWidget = Cast<ULimitTimeBase>(LimitTimeMonitor->GetWidgetFromName(FName("LimitTime")));
			}
		}
	}
}

void ALimitTimeMonitor::UpdatePlayTime()
{
	if (LimitTimeWidget)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS)
		{
			APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (!PC)
			{
				return;
			}

			if (GS->bPrimaryModifyPlayTime)
			{
				if (GS->CurrentPlayTime >= 0)
				{
					LimitTimeWidget->UpdatePlayTime(GS->CurrentPlayTime);
				}
			}
			else
			{
				if (GS->CurrentPlayTime >= GS->OriginPlayTime)
				{
					LimitTimeWidget->UpdatePlayTime(GS->OriginPlayTime);
				}
				else
				{
					// Initalize 전 59초 뜰 경우 막기 위해 OriginPlayTime으로 고정 후 값 변경
					LimitTimeWidget->UpdatePlayTime(GS->OriginPlayTime);
					LimitTimeWidget->UpdatePlayTime(GS->CurrentPlayTime);
				}
			}
		}
	}
}
