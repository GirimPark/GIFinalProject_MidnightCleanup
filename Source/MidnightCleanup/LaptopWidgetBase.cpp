// Fill out your copyright notice in the Description page of Project Settings.


#include "LaptopWidgetBase.h"
#include "RequestSelectionBase.h"
#include "LoadingWidgetBase.h"
#include "Components/WidgetSwitcher.h"

void ULaptopWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	RequestSelectionBox = Cast<URequestSelectionBase>(GetWidgetFromName(TEXT("RequestSelection")));
	LoadingBox = Cast<ULoadingWidgetBase>(GetWidgetFromName(TEXT("Loading")));
	if (LoadingBox)
	{
		LoadingBox->SetVisibility(ESlateVisibility::Hidden);
	}

	WidgetSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("WidgetSwitcher")));
	if(!WidgetSwitcher)
	{
		UE_LOG(LogTemp, Error, TEXT("GetWidgetSwitcher Failed(ULaptopWidgetBase::NativeConstruct)"));
	}

	OnVisibilityChanged.AddDynamic(this, &ULaptopWidgetBase::OnLaptopWidgetShowed);
}

void ULaptopWidgetBase::EnableLoadingScreen()
{
	if (LoadingBox)
	{
		LoadingBox->SetVisibility(ESlateVisibility::Visible);
		LoadingBox->VisibleLoadingScreen();
	}
}

void ULaptopWidgetBase::OnLaptopWidgetShowed(ESlateVisibility NewVisibility)
{
	if(NewVisibility == ESlateVisibility::Visible)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}
