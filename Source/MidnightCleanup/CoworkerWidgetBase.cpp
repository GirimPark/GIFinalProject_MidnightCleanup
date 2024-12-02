// Fill out your copyright notice in the Description page of Project Settings.


#include "CoworkerWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "InGamePS.h"
#include "PlayerCharacter.h"

void UCoworkerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	GradeBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("Grade")));
	UserIDBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserID")));
	AchievementBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("Achievement")));
	ThumbsUpButton = Cast<UButton>(GetWidgetFromName(TEXT("ThumbsUpBTN")));
	if (ThumbsUpButton)
	{
		ThumbsUpButton->OnClicked.AddDynamic(this, &UCoworkerWidgetBase::ClickedThumbsUpButton);
	}
	ThumbsUpLayoutBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("ThumbsUpLayout")));
	ThumbsUpDisableBox = Cast<UImage>(GetWidgetFromName(TEXT("ThumbsUpDisable")));
}

void UCoworkerWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	
}

void UCoworkerWidgetBase::SetGradeBox(FText InValue)
{
	if (GradeBox)
	{
		GradeBox->SetText(InValue);
	}
}

void UCoworkerWidgetBase::SetUserIDBox(FText InValue)
{
	if (UserIDBox)
	{
		UserIDBox->SetText(InValue);
	}
}

void UCoworkerWidgetBase::SetAchievementBox(FText InValue)
{
	if (AchievementBox)
	{
		AchievementBox->SetText(InValue);
	}
}

void UCoworkerWidgetBase::UpdateThumbsUpIcon(int32 InValue)
{
	if (ThumbsUpLayoutBox && InValue >= 0)
	{
		TArray<UWidget*> TempArr = ThumbsUpLayoutBox->GetAllChildren();
		for (int i = TempArr.Num() - 1; i > TempArr.Num() - 1 - InValue; i--)
		{
			UImage* Image = Cast<UImage>(TempArr[i]);
			if (Image)
			{
				FSlateBrush Brush = Image->GetBrush();
				if (ThumbsUpIconTexture)
				{
					Brush.SetResourceObject(ThumbsUpIconTexture);
					Image->SetBrush(Brush);
					Image->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void UCoworkerWidgetBase::HideThumbsUpButton()
{
	if (ThumbsUpButton&& ThumbsUpDisableBox)
	{
		ThumbsUpButton->SetVisibility(ESlateVisibility::Hidden);
		ThumbsUpDisableBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCoworkerWidgetBase::ClickedThumbsUpButton()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		if (Player && UserIDBox)
		{
			Player->SetThumbsUpNum(UserIDBox->GetText().ToString());
			HideThumbsUpButton();
		}
	}
}
