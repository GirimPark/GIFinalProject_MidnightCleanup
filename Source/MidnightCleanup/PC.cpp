// Fill out your copyright notice in the Description page of Project Settings.


#include "PC.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InGameWidgetBase.h"
#include "FurnitureProgressBase.h"
#include "RewardWidgetBase.h"
#include "RadialUIBase.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Seunggi/InGameGM.h"
#include "InteractionFurniture.h"
#include "ToolTipWidgetBase.h"
#include "CleanProgressMonitorBase.h"
#include "Seunggi/InGameGS.h"
#include "PauseWidgetBase.h"
#include "CaptionWidgetBase.h"
#include "GRNetworkSubsystem.h"
#include "Components\WidgetSwitcher.h"
#include "InGamePS.h"
#include "AimWidgetBase.h"
#include "LogicHandler.h"
#include "Net/UnrealNetwork.h"
#include "FinishWorkProgressBarWidgetBase.h"
#include "Seunggi\BasicPCM.h"
#include "Camera\CameraComponent.h"

void APC::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem && DefaultIMC)
	{
		SubSystem->AddMappingContext(DefaultIMC, 0);
	}

	if (IsLocalPlayerController())
	{
		AInGamePS* PS = GetPlayerState<AInGamePS>();
		if (PS)
		{
			PS->SetOwner(this);
		}

		DefaultMouseCursor = EMouseCursor::Default;
		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		if (InGameWidgetClass)
		{
			InGameWidget = CreateWidget<UInGameWidgetBase>(this, InGameWidgetClass);
			if (InGameWidget)
			{
				InGameWidget->AddToViewport();
				InGameWidget->RadialUIBox->SetVisibility(ESlateVisibility::Hidden);
				InGameWidget->FurnitureProgressBox->SetVisibility(ESlateVisibility::Hidden);
				InGameWidget->AimBox->SetVisibility(ESlateVisibility::Visible);
				InGameWidget->ToolTipBox->SetVisibility(ESlateVisibility::Visible);
			}
		}

		if (RewardWidgetClass)
		{
			RewardWidget = CreateWidget<URewardWidgetBase>(this, RewardWidgetClass);
			if (RewardWidget)
			{
				RewardWidget->AddToViewport(10);
				RewardWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (CleanProgressWidgetClass)
		{
			CleanProgressWidget = CreateWidget<UCleanProgressMonitorBase>(this, CleanProgressWidgetClass);
			if (CleanProgressWidget)
			{
				CleanProgressWidget->AddToViewport();
				CleanProgressWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (PauseWidgetClass)
		{
			PauseWidget = CreateWidget<UPauseWidgetBase>(this, PauseWidgetClass);
			if (PauseWidget)
			{
				PauseWidget->AddToViewport(1);
				PauseWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (CaptionWidgetClass)
		{
			CaptionWidget = CreateWidget<UCaptionWidgetBase>(this, CaptionWidgetClass);
			if (CaptionWidget)
			{
				CaptionWidget->AddToViewport();
			}
		}

		if (FinishWidgetClass)
		{
			FinishWidget = CreateWidget<URewardWidgetBase>(this, FinishWidgetClass);
			if (FinishWidget)
			{
				FinishWidget->AddToViewport();
				FinishWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (FinishWorkProgressBarClass)
		{
			FinishWorkProgressBarWidget = CreateWidget<UFinishWorkProgressBarWidgetBase>(this, FinishWorkProgressBarClass);
			if (FinishWorkProgressBarWidget)
			{
				FinishWorkProgressBarWidget->AddToViewport();
				FinishWorkProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->OnEndGameRespond.AddDynamic(this, &APC::OnEndGameRespond);
}

void APC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APC, bEnableCaption);
	DOREPLIFETIME(APC, bFinishGame);
}

void APC::ModifyPlayTime(bool State)
{
	C2S_UpdatePlayTime(State);
}

void APC::HideInGameWidget()
{
	if (InGameWidget)
	{
		InGameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APC::HideInventoryWidget(APlayerCharacter* InPlayer)
{
	if (InPlayer)
	{	
		UpdateInventoryByUI(InPlayer);
		InPlayer->SetInventory(0);
		InPlayer->C2S_SetUpdateInventoryState(false);
		InPlayer->C2S_UpdatePreviousInventoryIndex();
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	InGameWidget->RadialUIBox->SetVisibility(ESlateVisibility::Hidden);

	ShowAimWidget(InPlayer);
}

void APC::ShowInventoryWidget(APlayerCharacter* InPlayer)
{
	HideAimWidget(InPlayer);

	if (InPlayer)
	{
		InGameWidget->UpdateInventoryByPlayer(InPlayer);
		InPlayer->C2S_SetUpdateInventoryState(true);
	}

	bShowMouseCursor = false;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetWidgetToFocus(InGameWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);

	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);

	InGameWidget->RadialUIBox->SetVisibility(ESlateVisibility::Visible);
}

void APC::HideAimWidget(APlayerCharacter* InPlayer)
{
	if (InGameWidget)
	{
		InGameWidget->AimBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APC::ShowAimWidget(APlayerCharacter* InPlayer)
{
	if (InGameWidget)
	{
		InGameWidget->AimBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void APC::HideRewardWidget()
{
	if (RewardWidget)
	{
		RewardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APC::ShowRewardWidget(APlayerCharacter* InPlayer)
{
	if (!RewardWidget->IsVisible())
	{
		if (FinishWidget->IsVisible())
		{
			FinishWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		
		RewardWidget->UpdateReward();

		RewardWidget->SetVisibility(ESlateVisibility::Visible);

		RewardWidget->PlayFadeInAnim();
	}
}

void APC::HideFurnitureProgressWidget()
{
	if (InGameWidget && InGameWidget->FurnitureProgressBox)
	{
		if (InGameWidget->FurnitureProgressBox->IsVisible())
		{
			InGameWidget->FurnitureProgressBox->SetVisibility(ESlateVisibility::Hidden);
			//UE_LOG(LogTemp, Warning, TEXT("Furniture Widget Hidden"));
		}
	}
}

void APC::ShowFurnitureProgressWidget()
{
	if (InGameWidget && InGameWidget->FurnitureProgressBox)
	{
		InGameWidget->FurnitureProgressBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void APC::HideCleanProgressWidget()
{
	if (CleanProgressWidget)
	{
		CleanProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APC::ShowCleanProgressWidget()
{
	if (CleanProgressWidget)
	{
		CleanProgressWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

bool APC::IsEnableRewardWidget()
{
	if (RewardWidget && RewardWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APC::IsEnablePauseWidget()
{
	if (PauseWidget && PauseWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APC::UpdateInventoryByUI(APlayerCharacter* InPlayer)
{
	if (InGameWidget)
	{
		InGameWidget->RadialUIBox->SetMousePos(InGameWidget->GetCurrentInventoryIndex());
	}

	if (InPlayer)
	{
		InPlayer->C2S_SetInventoryIndex(InGameWidget->GetCurrentInventoryIndex());
	}
}

bool APC::IsEnableInventoryWidget()
{
	if (!InGameWidget)
	{
		return false;
	}

	if (!InGameWidget->RadialUIBox)
	{
		return false;
	}

	return InGameWidget->RadialUIBox->IsVisible();
}

void APC::UpdateFurnitureProgressBar(float InMaxHp, float InCurrentHp)
{
	if (InGameWidget && InGameWidget->FurnitureProgressBox)
	{
		InGameWidget->FurnitureProgressBox->UpdatePercent(InCurrentHp / InMaxHp);
	}
}

void APC::UpdateToolTip(FText InText)
{
	if (InGameWidget && InGameWidget->ToolTipBox)
	{
		InGameWidget->ToolTipBox->UpdateScript(InText);
	}
}

void APC::ShowToolTipWidget()
{
	if (InGameWidget && InGameWidget->ToolTipBox)
	{
		InGameWidget->ToolTipBox->SetOpacity(1.f);
	}
}

void APC::HideToolTipWidget()
{
	if (InGameWidget && InGameWidget->ToolTipBox)
	{
		InGameWidget->ToolTipBox->SetOpacity(0.f);
	}
}

bool APC::IsEnableToolTipWidget()
{
	if (InGameWidget && InGameWidget->ToolTipBox)
	{
		if (InGameWidget->ToolTipBox->GetRenderOpacity() > 0)
		{
			return true;
		}

		return false;
	}
	else
	{
		return false;
	}
}

void APC::ShowPauseWidget()
{
	if (PauseWidget)
	{
		PauseWidget->DisableFriendInviteButton();
	
		PauseWidget->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		SetInputMode(InputMode);

		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);
	}
}

void APC::HidePauseWidget()
{
	if (PauseWidget)
	{
		HideToolTipWidget();

		PauseWidget->HidePopupWidget();

		PauseWidget->SetVisibility(ESlateVisibility::Hidden);

		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}

void APC::UpdateCleanProgressWidget()
{
	if (CleanProgressWidget)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS)
		{
			if (GS->bPrimaryModifyPlayTime)
			{
				if (GS->CurrentPlayTime >= 0)
				{
					CleanProgressWidget->UpdatePlayTime(GS->CurrentPlayTime);
				}
			}
			else
			{
				if (GS->CurrentPlayTime >= GS->OriginPlayTime)
				{
					CleanProgressWidget->UpdatePlayTime(GS->OriginPlayTime);
				}
				else
				{
					CleanProgressWidget->UpdatePlayTime(GS->CurrentPlayTime);
				}
			}
			
			CleanProgressWidget->UpdateStarIcon(GS->StarNum);

			CleanProgressWidget->UpdateCount(
				GS->EraseTextureDecalNum,
				GS->EraseOpacityDecalNum,
				GS->InitFurnitureCount - GS->BreakFurnitureNum,
				GS->InitTrashCount - GS->TrashNum,
				GS->WoodenPlankNum,
				GS->CleanedPropsNum,
				GS->InitTextureDecalCount,
				GS->InitOpacityDecalCount + GS->CreateOpacityDecalCount,
				GS->InitFurnitureCount,
				GS->InitTrashCount,
				GS->InitWoodCount,
				GS->InitPropsCount, 
				GS->CreateTextureDecalCount);
		}
	}
}

bool APC::IsEnableCleanProgressWidget()
{
	if (!CleanProgressWidget)
	{
		return false;
	}

	return CleanProgressWidget->IsVisible();
}

void APC::UpdateCaption(FText InText, float Speed, const FString& InSFXPath)
{
	if (CaptionWidget)
	{
		CaptionWidget->SetCaptionBox(InText, Speed, InSFXPath);
	}
}

void APC::ShowFinishWidget()
{
	HideInGameWidget();

	if (!FinishWidget->IsVisible())
	{
		FinishWidget->SetVisibility(ESlateVisibility::Visible);

		UWidgetSwitcher* WidgetSwitcher = Cast<UWidgetSwitcher>(FinishWidget->GetWidgetFromName(FName("WidgetSwitcher")));

		if (WidgetSwitcher)
		{
			WidgetSwitcher->SetActiveWidgetIndex(!bIsInTruck);
		}
	}
}

void APC::UpdateAimIcon(EIconType InType)
{
	if (InGameWidget)
	{
		InGameWidget->UpdateAimIcon(InType);
	}
}

void APC::ShowFinishWorkProgressBarWidget()
{
	if (!IsEnableFinishWorkProgressBarWidget())
	{
		FinishWorkProgressBarWidget->SetVisibility(ESlateVisibility::Visible);
		FinishWorkProgressBarWidget->ShowProgressBar();
	}
}

void APC::HideFinishWorkProgressBarWidget()
{
	if (IsEnableFinishWorkProgressBarWidget())
	{
		FinishWorkProgressBarWidget->HideProgressBar();
		FinishWorkProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool APC::IsEnableFinishWorkProgressBarWidget()
{
	if (!FinishWorkProgressBarWidget)
	{
		return false;
	}

	return FinishWorkProgressBarWidget->IsEnableProgressBar();
}

void APC::ApplyPostProcessEffect(FPostProcessSettings PostProcessSetting,bool bEnableEffect)
{
	if (PlayerCameraManager)
	{
		
		if (bEnableEffect)
		{
			PlayerCameraManager->AddCachedPPBlend(PostProcessSetting, 1.0f);
		}
		else
		{
			PlayerCameraManager->AddCachedPPBlend(PostProcessSetting, 0.0f);
		}
	}
}

void APC::SetEnableCaption(bool State)
{
	C2S_SetEnableCaption(State);
}

void APC::SetTutorialTime(int32 InTime)
{
	C2S_SetTutorialTime(InTime);
}

void APC::OpenInitGame()
{
	if (IsEnableRewardWidget())
	{
		C2S_OpenInitGame();
		HideRewardWidget();
	}
}

void APC::C2S_OpenInitGame_Implementation()
{
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM)
		return;

	if (GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->IsVerified())
	{
		GetGameInstance()->GetSubsystem<UGRNetworkSubsystem>()->GetLogicHandler()->RequestEndGame();
	}
	else
	{
		GM->OpenInitGame();
	}
}

bool APC::IsCurrentWorldName(const FString& InName)
{
	FString MapName = GetWorld()->GetMapName();

	int32 StartIndex = MapName.Find(TEXT("L_InGame"));
	if (StartIndex != INDEX_NONE)
	{
		MapName = MapName.RightChop(StartIndex);
	}

	return InName == MapName;
}

void APC::OnEndGameRespond()
{
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
		GM->OpenInitGame();
}

void APC::OnRep_FinishGame()
{
	UE_LOG(LogTemp, Warning, TEXT("%s : %d"), *GetName(), bFinishGame);

	if (!IsLocalController())
	{
		return;
	}

	if (!bFinishGame)
	{
		return;
	}

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetWidgetToFocus(RewardWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);
}

void APC::EmplaceAchievementData(FAchievementData InData)
{
	AInGamePS* PS = GetPlayerState<AInGamePS>();
	if (PS)
	{
		PS->C2S_EmplaceAchievementData(InData);
	}
}

void APC::RemoveFirstAchievementData()
{
	AInGamePS* PS = GetPlayerState<AInGamePS>();
	if (PS)
	{
		PS->S2A_RemoveFirstAchievementData();
	}
}

void APC::UpdateMostAchievementValue(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount)
{
	C2S_UpdateMostAchievementValue(InEnableTriggerCount, InMostCreateDecalCount, InMostEraseDecalCount, InMostOrganizePropsCount, InMostSpillBucketCount, InMostProximityTime, InMostBreakFurnitureCount, InMostStealCount, InMostSpawnBoxAndBucketCount);
}

void APC::UpdateOwnedAchievement()
{
	AInGamePS* PS = GetPlayerState<AInGamePS>();
	if (PS)
	{
		PS->C2S_UpdateAchievement();
	}
}

void APC::C2S_UpdateMostAchievementValue_Implementation(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount)
{
	S2A_UpdateMostAchievementValue(InEnableTriggerCount, InMostCreateDecalCount, InMostEraseDecalCount, InMostOrganizePropsCount, InMostSpillBucketCount, InMostProximityTime, InMostBreakFurnitureCount, InMostStealCount, InMostSpawnBoxAndBucketCount);
}

void APC::S2A_UpdateMostAchievementValue_Implementation(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount)
{
	AInGameGS* GS = Cast<AInGameGS>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->UpdateMostAchievementValue(InEnableTriggerCount, InMostCreateDecalCount, InMostEraseDecalCount, InMostOrganizePropsCount, InMostSpillBucketCount, InMostProximityTime, InMostBreakFurnitureCount, InMostStealCount, InMostSpawnBoxAndBucketCount);
	}
}

void APC::C2S_SetTutorialTime_Implementation(int32 InTime)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	GS->PlayTime = GS->OriginPlayTime + InTime;
	GS->CurrentPlayTime = GS->PlayTime;
	GS->OnRep_UpdateTime();
	
	S2A_SetTutorialTime(InTime);
}

void APC::S2A_SetTutorialTime_Implementation(int32 InTime)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->SetTutorialTime(InTime);
	}
}

void APC::C2S_SetEnableCaption_Implementation(bool State)
{
	S2A_SetEnableCaption(State);
}

void APC::S2A_SetEnableCaption_Implementation(bool State)
{
	bEnableCaption = State;
}

void APC::C2S_UpdatePlayTime_Implementation(bool State)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	GS->bPrimaryModifyPlayTime = true;

	UE_LOG(LogTemp, Warning, TEXT("C2S_UpdatePlayTime_Implementation"));

	if (State)
	{
		GS->PlayTime = GS->OriginPlayTime + GS->TutorialTime + GS->ModifyTime;
		GS->CurrentPlayTime += GS->ModifyTime;
		GS->OriginPlayTime += GS->ModifyTime;
		GS->OnRep_UpdateTime();
	}
	else
	{
		if (GS->CurrentPlayTime > GS->ModifyTime)
		{
			GS->PlayTime = GS->OriginPlayTime + GS->TutorialTime - GS->ModifyTime;
			GS->CurrentPlayTime -= GS->ModifyTime;
			GS->OriginPlayTime -= GS->ModifyTime;
			GS->OnRep_UpdateTime();
		}
	}
}
