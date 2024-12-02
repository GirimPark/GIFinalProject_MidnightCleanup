// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestSelectionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InitGamePC.h"
#include "GI.h"
#include "GRNetworkSubsystem.h"
#include "InitGameGM.h"
#include "LogicHandler.h"
#include "Seunggi/InGameGS.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "PlayerCharacter.h"

void URequestSelectionBase::NativeConstruct()
{
	Super::NativeConstruct();

	StageBox1 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Stage1")));
	if (StageBox1)
	{
		StageBox1->OnPressed.AddDynamic(this, &URequestSelectionBase::OnStage1ButtonPressed);
		StageBox1->OnReleased.AddDynamic(this, &URequestSelectionBase::OnStage1ButtonReleased);
	}

	StageBox2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Stage2")));
	if (StageBox2)
	{
		StageBox2->OnPressed.AddDynamic(this, &URequestSelectionBase::OnStage2ButtonPressed);
		StageBox2->OnReleased.AddDynamic(this, &URequestSelectionBase::OnStage2ButtonReleased);
	}

	StageBox3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Stage3")));
	if (StageBox3)
	{
		StageBox3->OnPressed.AddDynamic(this, &URequestSelectionBase::OnStage3ButtonPressed);
		StageBox3->OnReleased.AddDynamic(this, &URequestSelectionBase::OnStage3ButtonReleased);
	}


	if (!ProgressMaterialClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressMaterialClass is Empty"));
		return;
	}
	ProgressMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ProgressMaterialClass);
	if (!ProgressMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressMaterial is Empty"));
		return;
	}
	ProgressMaterial->SetScalarParameterValue(FName("Percent"), 0.f);


	ProgressBarImage1 = Cast<UImage>(GetWidgetFromName(TEXT("ProgressBar_Stage1")));
	ProgressBarImage1->SetRenderOpacity(0.f);
	ProgressBarImage1->SetBrushFromMaterial(ProgressMaterial);

	ProgressBarImage2 = Cast<UImage>(GetWidgetFromName(TEXT("ProgressBar_Stage2")));
	ProgressBarImage2->SetRenderOpacity(0.f);
	ProgressBarImage2->SetBrushFromMaterial(ProgressMaterial);

	ProgressBarImage3 = Cast<UImage>(GetWidgetFromName(TEXT("ProgressBar_Stage3")));
	ProgressBarImage3->SetRenderOpacity(0.f);
	ProgressBarImage3->SetBrushFromMaterial(ProgressMaterial);


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
		ReturnBox->OnClicked.AddDynamic(this, &URequestSelectionBase::ClickedReturnButton);
	}

	CashBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("CashText")));
	if (CashBox)
	{
		UGI* GI = Cast<UGI>(GetGameInstance());
		if (GI)
		{
			FString CashTemp = FString::FromInt(GI->Cash) + TEXT("\uFFE5");
			CashBox->SetText(FText::FromString(CashTemp));
		}
	}

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnStartGameRespond.AddDynamic(this, &URequestSelectionBase::OnStartGameRespond);
}

void URequestSelectionBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bButton1Pressed)
		OnStage1ButtonPressed();
	if(bButton2Pressed)
		OnStage2ButtonPressed();
	if (bButton3Pressed)
		OnStage3ButtonPressed();
}

void URequestSelectionBase::UpdateProgressBar(TObjectPtr<UImage>& ProgressBarImage, float InRatio)
{
	if (ProgressMaterial)
	{
		ProgressMaterial->SetScalarParameterValue(FName("Percent"), InRatio);
		ProgressBarImage->SetBrushFromMaterial(ProgressMaterial);
	}

	if (InRatio >= 1.f)
	{
		ProgressBarImage->SetRenderOpacity(0.f);

		EnableLoadingScreen();

		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (!GS)
		{
			UE_LOG(LogTemp, Error, TEXT("Cast to IngameGS Failed"));
			return;
		}

		AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if(!PC)
		{
			UE_LOG(LogTemp, Error, TEXT("Cast to InitgamePC Failed"));
			return;
		}

		if (ProgressBarImage == ProgressBarImage1)
			PC->SetOpenLevelName(TEXT("L_InGame_2p"));
		else if (ProgressBarImage == ProgressBarImage2)
			PC->SetOpenLevelName(TEXT("L_InGame_3p"));
		else if(ProgressBarImage == ProgressBarImage3)
			PC->SetOpenLevelName(TEXT("L_InGame"));
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OpenLevelName isnt assigned"));
			return;
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URequestSelectionBase::OpenInGame, GS->LoadingTime, false);
	}
}

void URequestSelectionBase::OnStage1ButtonPressed()
{
	if(!bButton1Pressed)
	{
		bButton1Pressed = true;
		ProgressBarImage1->SetRenderOpacity(1.f);
		ButtonFirstPressedTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	}
	
	UpdateProgressBar(ProgressBarImage1, (UGameplayStatics::GetTimeSeconds(GetWorld()) - ButtonFirstPressedTime) / SelectMapTime);
}

void URequestSelectionBase::OnStage1ButtonReleased()
{
	bButton1Pressed = false;
	ProgressBarImage1->SetRenderOpacity(0.f);
	UpdateProgressBar(ProgressBarImage1, 0.f);
}

void URequestSelectionBase::OnStage2ButtonPressed()
{
	if (!bButton2Pressed)
	{
		bButton2Pressed = true;
		ProgressBarImage2->SetRenderOpacity(1.f);
		ButtonFirstPressedTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	}

	UpdateProgressBar(ProgressBarImage2, (UGameplayStatics::GetTimeSeconds(GetWorld()) - ButtonFirstPressedTime) / SelectMapTime);
}

void URequestSelectionBase::OnStage2ButtonReleased()
{
	bButton2Pressed = false;
	ProgressBarImage2->SetRenderOpacity(0.f);
	UpdateProgressBar(ProgressBarImage2, 0.f);
}

void URequestSelectionBase::OnStage3ButtonPressed()
{
	if (!bButton3Pressed)
	{
		bButton3Pressed = true;
		ProgressBarImage3->SetRenderOpacity(1.f);
		ButtonFirstPressedTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	}

	UpdateProgressBar(ProgressBarImage3, (UGameplayStatics::GetTimeSeconds(GetWorld()) - ButtonFirstPressedTime) / SelectMapTime);
}

void URequestSelectionBase::OnStage3ButtonReleased()
{
	bButton3Pressed = false;
	ProgressBarImage3->SetRenderOpacity(0.f);
	UpdateProgressBar(ProgressBarImage3, 0.f);
}


void URequestSelectionBase::OnStartGameRespond()
{
	AInitGamePC* PC = Cast<AInitGamePC>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OpenInGame();
	}
}

void URequestSelectionBase::ClickedReturnButton()
{
	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->HideLaptopWidget();
	}
}

void URequestSelectionBase::OpenInGame()
{
	if (GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
		GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestStartGame();
	else
		OnStartGameRespond();
}

void URequestSelectionBase::EnableLoadingScreen()
{
	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		PC->EnableLoadingScreen();
	}
}
