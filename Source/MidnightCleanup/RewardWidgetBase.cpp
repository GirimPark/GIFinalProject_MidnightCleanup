// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GI.h"
#include "PC.h"
#include "Seunggi/InGameGS.h"
#include "InGamePS.h"
#include "CoworkerWidgetBase.h"
#include "Seunggi/InGameGM.h"
#include "GRNetworkSubsystem.h"

void URewardWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UserIDBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserID")));
	if (UserIDBox)
	{
		UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserIDBox->SetText(FText::FromString(GI->GetUserID()));
		}
	}

	ReturnBox = Cast<UButton>(GetWidgetFromName(TEXT("ReturnBTN")));
	if (ReturnBox)
	{
		ReturnBox->OnClicked.AddDynamic(this, &URewardWidgetBase::ClickedReturnButton);
	}

	CleaningRewardBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("CleaningReward")));
	AdditionalRewardBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("AdditionalReward")));
	TransportationRewardBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TransportationReward")));

	StarLayoutBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("StarLayout")));
	TotalRewardBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalReward")));
	TotalRewardBox_RightSide = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalReward_RightSide")));
	RewardScriptBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("RewardScript")));
	CoworkerListBox = Cast<UGridPanel>(GetWidgetFromName(TEXT("CoworkerList")));
}

void URewardWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCoworkerList();
}

void URewardWidgetBase::ClickedReturnButton()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickedReturnButton"));

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->OpenInitGame();
	}

	/*FString LevelName;
	if (GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
	{
		LevelName = TEXT("L_OutGame");
		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);
	}
	else
	{
		LevelName = TEXT("L_Title");
		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);
	}*/
}

void URewardWidgetBase::UpdateReward()
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		UpdateStarIcon(GS->StarNum);

		if (!GS->RewardScripts.IsEmpty())
		{
			FText Temp = FText::FromString(GS->RewardScripts[GS->StarNum]);
			UpdateRewardScript(Temp);
		}
		
		UpdateCleaningFee(GS->FinalCleaningFee);
		UpdateAdditionalFee(GS->FinalAdditionalFee);
		UpdateTransportationFee(GS->FinalTransportationFee);
		UpdateTotalReward(GS->FinalCleaningFee + GS->FinalAdditionalFee + GS->FinalTransportationFee);
		InitCoworkerList();
	}
}

void URewardWidgetBase::UpdateStarIcon(int32 InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("[STAR ICON NUM]** %d"), InValue);
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

void URewardWidgetBase::UpdateTotalReward(int32 InValue)
{
	if (TotalRewardBox && TotalRewardBox_RightSide)
	{
		FString CashTemp = FString::FromInt(InValue) + TEXT("\uFFE5");
		TotalRewardBox->SetText(FText::FromString(CashTemp));
		TotalRewardBox_RightSide->SetText(FText::FromString(CashTemp));

		UGI* GI = Cast<UGI>(GetGameInstance());

		if (InValue >= 0)
		{
			GI->Cash = InValue;
		}
		else
		{
			GI->Cash = 0;
		}
	}
}

void URewardWidgetBase::UpdateCleaningFee(int32 InValue)
{
	if (CleaningRewardBox)
	{
		FString CashTemp = FString::FromInt(InValue) + TEXT("\uFFE5");
		CleaningRewardBox->SetText(FText::FromString(CashTemp));
	}
}

void URewardWidgetBase::UpdateAdditionalFee(int32 InValue)
{
	if (AdditionalRewardBox)
	{
		FString CashTemp = FString::FromInt(InValue) + TEXT("\uFFE5");
		AdditionalRewardBox->SetText(FText::FromString(CashTemp));
	}
}

void URewardWidgetBase::UpdateTransportationFee(int32 InValue)
{
	if (TransportationRewardBox)
	{
		FString CashTemp = FString::FromInt(InValue) + TEXT("\uFFE5");
		TransportationRewardBox->SetText(FText::FromString(CashTemp));
	}
}

void URewardWidgetBase::UpdateRewardScript(const FText& InReward)
{
	if (RewardScriptBox)
	{
		RewardScriptBox->SetText(InReward);
	}
}

void URewardWidgetBase::InitCoworkerList()
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;
	}

	if (!CoworkerListBox)
	{
		return;
	}

	TArray<APlayerState*> Players = GS->PlayerArray;

	if (!Players.IsEmpty())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			AInGamePS* OwnedPS = PC->GetPlayerState<AInGamePS>();
			if (OwnedPS)
			{
				CoworkerList.Emplace(OwnedPS);

				for (int i = 0; i < Players.Num(); i++)
				{
					AInGamePS* PS = Cast<AInGamePS>(Players[i]);

					if (PS)
					{
						if (!(PS == OwnedPS))
						{
							CoworkerList.Emplace(PS);
						}
					}
				}
			}
		}
	}
}

void URewardWidgetBase::UpdateCoworkerList()
{	
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;
	}

	if (!CoworkerListBox)
	{
		return;
	}

	/*TArray<APlayerState*> Players = GS->PlayerArray;

	if (!Players.IsEmpty())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			AInGamePS* OwnedPS = PC->GetPlayerState<AInGamePS>();
			if (OwnedPS)
			{
				CoworkerList.Emplace(OwnedPS);

				for (int i = 0; i < Players.Num(); i++)
				{
					AInGamePS* PS = Cast<AInGamePS>(Players[i]);

					if (PS)
					{
						if (!(PS == OwnedPS))
						{
							CoworkerList.Emplace(PS);
						}
					}
				}
			}
		}
	}*/
	
	TArray<UPanelSlot*> TempArr = CoworkerListBox->GetSlots();
	if (!CoworkerList.IsEmpty() && !TempArr.IsEmpty())
	{
		for (int i = 0; i < CoworkerList.Num(); i++)
		{
			UCoworkerWidgetBase* Coworker = Cast<UCoworkerWidgetBase>(TempArr[i]->Content);
			if (Coworker)
			{
				if (i == 0)
				{
					Coworker->HideThumbsUpButton();
				}

				Coworker->SetUserIDBox(FText::FromString(CoworkerList[i]->UserID));
				Coworker->UpdateThumbsUpIcon(CoworkerList[i]->ThumbsUpNum);
				Coworker->SetRenderOpacity(1.0f);

				if (GS)
				{
					Coworker->SetAchievementBox(GS->GetAchievementByUserID(CoworkerList[i]->UserID));
				}
			}
		}
	}
}
