// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanProgressMonitorBase.h"
#include "Seunggi/InGameGM.h"	
#include "Seunggi/InGameGS.h"	
#include "Kismet/GameplayStatics.h"
#include "CleanProgressBarBase.h"
#include "LimitTimeBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UCleanProgressMonitorBase::NativeConstruct()
{
	Super::NativeConstruct();

	//UE_LOG(LogTemp, Warning, TEXT("UCleanProgressMonitorBase::NativeConstruct"));

	MopDecalProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_MopDecal")));
	SpongeDecalProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_SpongeDecal")));
	FurnitureProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Furniture")));
	TrashProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Trash")));
	WoodProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Wood")));
	PropsProgressBox = Cast<UCleanProgressBarBase>(GetWidgetFromName(TEXT("ProgressBar_Props")));
	LimitTimeBox = Cast<ULimitTimeBase>(GetWidgetFromName(TEXT("LimitTime")));
	StarLayoutBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("StarLayout_InGame")));
}

void UCleanProgressMonitorBase::UpdateCount(int32 InMopDecal, int32 InSpongeDecal, int32 InFurniture, int32 InTrash, int32 InWood, int32 InProps, int32 InitMopDecal, int32 InitSpongeDecal, int32 InitFurniture, int32 InitTrash, int32 InitWood, int32 InitProps, int32 InCreateTextureDecal)
{
	if (MopDecalProgressBox)
	{
		MopDecalProgressBox->UpdateCount(InMopDecal, InitMopDecal + InCreateTextureDecal);
	}

	if (SpongeDecalProgressBox)
	{
		SpongeDecalProgressBox->UpdateCount(InSpongeDecal, InitSpongeDecal);
	}

	if (FurnitureProgressBox)
	{
		FurnitureProgressBox->UpdateCount(InFurniture, InitFurniture);
	}

	if (TrashProgressBox)
	{
		TrashProgressBox->UpdateCount(InTrash, InitTrash);
	}

	if (WoodProgressBox)
	{
		WoodProgressBox->UpdateCount(InWood, InitWood);
	}

	if (PropsProgressBox)
	{
		PropsProgressBox->UpdateCount(InProps, InitProps);
	}
}

void UCleanProgressMonitorBase::UpdatePlayTime(int32 InSeconds)
{
	if (LimitTimeBox)
	{
		LimitTimeBox->UpdatePlayTime(InSeconds);
	}
}

void UCleanProgressMonitorBase::UpdateStarIcon(int32 InValue)
{
	if (StarLayoutBox && InValue >= 0)
	{
		TArray<UWidget*> TempArr = StarLayoutBox->GetAllChildren();
		for (int i = 0; i < InValue; i++)
		{
			UImage* Image = Cast<UImage>(TempArr[i]);
			if (Image)
			{
				FSlateBrush Brush = Image->GetBrush();
				if (StarIconTexture)
				{
					Brush.SetResourceObject(StarIconTexture);
					Image->SetBrush(Brush);
				}
			}
		}

		for (int j = InValue; j < TempArr.Num(); j++)
		{
			UImage* Image = Cast<UImage>(TempArr[j]);
			if (Image)
			{
				FSlateBrush Brush = Image->GetBrush();
				if (StarBackgroundIconTexture)
				{
					Brush.SetResourceObject(StarBackgroundIconTexture);
					Image->SetBrush(Brush);
				}
			}
		}
	}
}
