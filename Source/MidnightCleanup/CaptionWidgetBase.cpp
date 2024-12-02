// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptionWidgetBase.h"
#include "Components/TextBlock.h"

void UCaptionWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	CaptionBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("CaptionText")));

	PlayTutorialTextAnim();
}

void UCaptionWidgetBase::SetCaptionBox(FText InText, float Speed, const FString& InSFXPath)
{
	UE_LOG(LogTemp, Warning, TEXT("UCaptionWidgetBase:UpdateCaption"));

	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), this, *InSFXPath));

	if (CaptionBox)
	{
		CaptionBox->SetText(InText);
		PlayCaptionTextAnim(Speed, Sound);
	}
}