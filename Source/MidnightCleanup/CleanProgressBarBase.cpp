// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanProgressBarBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UCleanProgressBarBase::NativeConstruct()
{
	Super::NativeConstruct();

	CountBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
	ProgressBarBox = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
}

void UCleanProgressBarBase::UpdateCount(int32 InCount, int32 InitCount)
{
	if (CountBox)
	{
		CountBox->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), InCount, InitCount)));
	}

	if (ProgressBarBox)
	{
		if (InitCount != 0)
		{
			ProgressBarBox->SetPercent((float)(InCount) / (float)InitCount);
		}
		else
		{
			ProgressBarBox->SetPercent(0.f);
		}
	}
}
