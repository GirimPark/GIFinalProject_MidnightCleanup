// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidgetBase.h"
#include "Components/Image.h"

void ULoadingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	BackgroundBox = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	if (BackgroundBox)
	{
		BackgroundBox->SetRenderOpacity(0.0f);
	}
}

void ULoadingWidgetBase::VisibleLoadingScreen()
{
	if (BackgroundBox)
	{
		if (BackgroundArray.IsEmpty())
		{
			return;
		}

		int32 Index = FMath::RandRange(0, BackgroundArray.Num() - 1);
		FSlateBrush Brush = BackgroundBox->GetBrush();
		Brush.SetResourceObject(BackgroundArray[Index]);
		BackgroundBox->SetBrush(Brush);
		BackgroundBox->SetRenderOpacity(1.0f);
	}
}

