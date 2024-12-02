// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishWorkProgressBarWidgetBase.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "PlayerCharacter.h"
#include "Seunggi/InGameGS.h"
#include "Kismet/GameplayStatics.h"

void UFinishWorkProgressBarWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ProgressBarBox = Cast<UImage>(GetWidgetFromName(TEXT("ProgressBar_FinishWork")));
	if (ProgressBarBox)
	{
		ProgressBarBox->SetRenderOpacity(0.f);
	}

	if (ProgressMaterialClass)
	{
		ProgressMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ProgressMaterialClass);

		if (ProgressMaterial)
		{
			ProgressMaterial->SetScalarParameterValue(FName("Percent"), 0.f);

			if (ProgressBarBox)
			{
				ProgressBarBox->SetBrushFromMaterial(ProgressMaterial);
			}
		}
	}
}

void UFinishWorkProgressBarWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

bool UFinishWorkProgressBarWidgetBase::IsEnableProgressBar()
{
	if (ProgressBarBox->GetRenderOpacity() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UFinishWorkProgressBarWidgetBase::ShowProgressBar()
{
	if (ProgressBarBox)
	{
		ProgressBarBox->SetRenderOpacity(1.f);
	}
}

void UFinishWorkProgressBarWidgetBase::HideProgressBar()
{
	if (ProgressMaterial)
	{
		ProgressMaterial->SetScalarParameterValue(FName("Percent"), 0.f);

		if (ProgressBarBox)
		{
			ProgressBarBox->SetBrushFromMaterial(ProgressMaterial);
			ProgressBarBox->SetRenderOpacity(0.f);
		}
	}
}

void UFinishWorkProgressBarWidgetBase::UpdateProgressBar(float InRatio, APlayerCharacter* InPlayer)
{
	if (ProgressMaterial)
	{
		ProgressMaterial->SetScalarParameterValue(FName("Percent"), InRatio);

		if (ProgressBarBox)
		{
			ProgressBarBox->SetBrushFromMaterial(ProgressMaterial);
		}
	}

	if (InRatio >= 1.f)
	{
		if (InPlayer)
		{
			InPlayer->EnableKeypad();
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
