// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGS.h"
#include "InGameGM.h"
#include "Net\UnrealNetwork.h"
#include "../CleanProgressMonitor.h"
#include "../LimitTimeMonitor.h"
#include "../CleanProgressMonitorBase.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerCharacter.h"
#include "../GI.h"
#include "../PC.h"
#include "Camera/CameraActor.h"
#include "../InGamePS.h"
#include "Spawner.h"
#include "../InteractionBox.h"
#include "../InteractionDecal.h"
#include "../InteractionBucket.h"

FAchievementData* AInGameGS::GetAchievementByID(FName ID)
{
	if (AchievementDataTable)
	{
		const FString ContextString(TEXT("Achievement Context"));
		return AchievementDataTable->FindRow<FAchievementData>(ID, ContextString, true);
	}
	else
	{
		return nullptr;
	}
}

FAchievementData* AInGameGS::GetSpecialAchievementByID(FName ID)
{
	if (SpecialAchievementDataTable)
	{
		const FString ContextString(TEXT("Speical Achievement Context"));
		return SpecialAchievementDataTable->FindRow<FAchievementData>(ID, ContextString, true);
	}
	else
	{
		return nullptr;
	}
}

FToolTipData* AInGameGS::GetToolTipByID(FName ID)
{
	if (ToolTipDataTable)
	{
		const FString ContextString(TEXT("Tooltip Context"));
		return ToolTipDataTable->FindRow<FToolTipData>(ID, ContextString, true);
	}
	else
	{
		return nullptr;
	}
}

FCaptionData* AInGameGS::GetCaptionByID(FName ID)
{
	if (CaptionDataTable)
	{
		const FString ContextString(TEXT("Caption Context"));
		return CaptionDataTable->FindRow<FCaptionData>(ID, ContextString, true);
	}
	else
	{
		return nullptr;
	}
}

void AInGameGS::OnRep_UpdateCount()
{
	UpdateStar();

	TArray<AActor*> Monitors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACleanProgressMonitor::StaticClass(), Monitors);
	if (Monitors.Num() > 0)
	{
		ACleanProgressMonitor* Monitor = Cast<ACleanProgressMonitor>(Monitors[0]);
		if (Monitor)
		{
			Monitor->UpdateCount();

		}
	}
	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), Spawners);
	for (auto& Spawner : Spawners)
	{
		ASpawner* Sp = Cast<ASpawner>(Spawner);
		if (Sp && Sp->SpawnObject)
		{
			if (Sp->SpawnObject->IsChildOf(AInteractionBox::StaticClass()))
			{
				Sp->CurrentObjectNum = BoxNum;
			}
			else
			{
				Sp->CurrentObjectNum = BucketNum;
			}
		}
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player)
	{
		return;
	}

	APC* PC = Cast<APC>(Player->GetController());
	if (!PC)
	{
		return;
	}

	if (CreateTextureDecalCount + InitTextureDecalCount - EraseTextureDecalNum <= 0)
	{
		if (!PC->bPrimaryCleanAllMopDecal)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_CleanAllMopDecal")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryCleanAllMopDecal = true;
			}
		}
	}

	if (CreateOpacityDecalCount + InitOpacityDecalCount - EraseOpacityDecalNum <= 0)
	{
		if (!PC->bPrimaryCleanAllSpongeDecal)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_CleanAllSpongeDecal")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryCleanAllSpongeDecal = true;
			}
		}
	}

	if (TrashNum <= 0)
	{
		if (!PC->bPrimaryCleanAllTrash)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_CleanAllTrash")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryCleanAllTrash = true;
			}
		}
	}

	if (InitPropsCount - CleanedPropsNum <= 0)
	{
		if (!PC->bPrimaryCleanAllProps)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_CleanAllProps")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryCleanAllProps = true;
			}
		}
	}

	switch (StarNum)
	{
	case 1:
	{
		if (!PC->bPrimaryStar_1)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_Star_1")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryStar_1 = true;
			}
		}
	}
	break;
	case 2:
	{
		if (!PC->bPrimaryStar_2)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_Star_2")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryStar_2 = true;
			}
		}
	}
	break;
	case 3:
	{
		if (!PC->bPrimaryStar_3)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_Star_3")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryStar_3 = true;
			}
		}
	}
	break;
	case 4:
	{
		if (!PC->bPrimaryStar_4)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_Star_4")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryStar_4 = true;
			}
		}
	}
	break;
	case 5:
	{
		if (!PC->bPrimaryStar_5)
		{
			FCaptionData* Data = GetCaptionByID(FName(TEXT("PrimaryNotify_Star_5")));
			if (Data)
			{
				Player->UpdateCaption(Data, 1.0f);
				PC->bPrimaryStar_5 = true;
			}
		}
	}
	break;
	}
}

void AInGameGS::OnRep_UpdateTime()
{
	if (TimeMileStones.Num() > 0)
	{
		if (TimeMileStones.Find(CurrentPlayTime))
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Player)
			{
				FCaptionData* Data = GetCaptionByID(TimeMileStones[CurrentPlayTime]);
				if (Data)
				{
					Player->UpdateCaption(Data, 2.0f);
					TimeMileStones.Remove(CurrentPlayTime);
				}
			}
		}
	}

	TArray<AActor*> Monitors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALimitTimeMonitor::StaticClass(), Monitors);
	if (Monitors.Num() > 0)
	{
		ALimitTimeMonitor* Monitor = Cast<ALimitTimeMonitor>(Monitors[0]);
		if (Monitor)
		{
			Monitor->UpdatePlayTime();
		}
	}
}

void AInGameGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGS, TrashNum);
	DOREPLIFETIME(AInGameGS, EraseTextureDecalNum);
	DOREPLIFETIME(AInGameGS, EraseOpacityDecalNum);
	DOREPLIFETIME(AInGameGS, WoodenPlankNum);
	DOREPLIFETIME(AInGameGS, BreakFurnitureNum);
	DOREPLIFETIME(AInGameGS, CleanedPropsNum);
	DOREPLIFETIME(AInGameGS, BoxNum);
	DOREPLIFETIME(AInGameGS, BucketNum);

	DOREPLIFETIME(AInGameGS, InitFurnitureCount);
	DOREPLIFETIME(AInGameGS, InitTextureDecalCount);
	DOREPLIFETIME(AInGameGS, CreateTextureDecalCount);
	DOREPLIFETIME(AInGameGS, CreateOpacityDecalCount);
	DOREPLIFETIME(AInGameGS, InitOpacityDecalCount);
	DOREPLIFETIME(AInGameGS, InitTrashCount);
	DOREPLIFETIME(AInGameGS, InitWoodCount);
	DOREPLIFETIME(AInGameGS, InitPropsCount);

	DOREPLIFETIME(AInGameGS, OriginPlayTime);
	DOREPLIFETIME(AInGameGS, PlayTime);
	DOREPLIFETIME(AInGameGS, CurrentPlayTime);
	DOREPLIFETIME(AInGameGS, TutorialTime);

	DOREPLIFETIME(AInGameGS, MostCreateDecalCount);
	DOREPLIFETIME(AInGameGS, MostEraseDecalCount);
	DOREPLIFETIME(AInGameGS, MostOrganizePropsCount);
	DOREPLIFETIME(AInGameGS, MostSpillBucketCount);
	DOREPLIFETIME(AInGameGS, MostProximityTime);
	DOREPLIFETIME(AInGameGS, MostEnableTriggerCount);
	DOREPLIFETIME(AInGameGS, MostBreakFurnitureCount);
	DOREPLIFETIME(AInGameGS, MostStealCount);
	DOREPLIFETIME(AInGameGS, MostBoxAndBucketSpawnCount);

	DOREPLIFETIME(AInGameGS, FinalCleaningFee);
	DOREPLIFETIME(AInGameGS, FinalAdditionalFee);
	DOREPLIFETIME(AInGameGS, FinalTransportationFee);
	DOREPLIFETIME(AInGameGS, StarNum);

	DOREPLIFETIME(AInGameGS, LoadingTime);
	DOREPLIFETIME(AInGameGS, PressKeypadTime);

	DOREPLIFETIME(AInGameGS, bPrimaryModifyPlayTime);

	DOREPLIFETIME(AInGameGS, PlayerNum);
}

void AInGameGS::BeginPlay()
{
	Super::BeginPlay();

	if (TutorialTime <= 0)
	{
		OriginPlayTime = PlayTime;
	}
	
	CurrentPlayTime = PlayTime;
	OnRep_UpdateTime();

	RankedAchievements.Empty();
	TArray<FAchievementData*>& Temp = RankedAchievements;

	if (AchievementDataTable)
	{
		AchievementDataTable->GetAllRows<FAchievementData>(TEXT("Fetching Achievements"), Temp);
		SortAchievement(Temp);
	}
}

void AInGameGS::UpdateStar()
{
	int32 DirtyNum = (InitTextureDecalCount - EraseTextureDecalNum + CreateTextureDecalCount) + (InitOpacityDecalCount - EraseOpacityDecalNum + CreateOpacityDecalCount) + TrashNum + WoodenPlankNum;

	if (DirtyNum >= StarCondition_0)
	{
		StarNum = 0;
	}
	else
	{
		if (StarCondition_1.MinValue <= DirtyNum && DirtyNum <= StarCondition_1.MaxValue)
		{
			StarNum = 1;
		}
		else
		{
			if (StarCondition_2.MinValue <= DirtyNum && DirtyNum <= StarCondition_2.MaxValue)
			{
				StarNum = 2;
			}
			else
			{
				if (StarCondition_3.MinValue <= DirtyNum && DirtyNum <= StarCondition_3.MaxValue)
				{
					StarNum = 3;
				}
				else
				{
					if (StarCondition_4.MinValue <= DirtyNum && DirtyNum <= StarCondition_4.MaxValue)
					{
						StarNum = 4;
					}
					else
					{
						if (DirtyNum <= 0 && (InitPropsCount <= CleanedPropsNum))
						{
							StarNum = 5;
						}
						else
						{
							StarNum = 4;
						}
					}
				}
			}
		}
	}
}

void AInGameGS::ShowRewardWidget()
{
	APC* PlayerController = Cast<APC>(UGameplayStatics::GetPlayerController(this, 0));
	//UE_LOG(LogTemp, Warning, TEXT("PC : %s"), *PlayerController->GetName());
	if (PlayerController)
	{
		PlayerController->ShowRewardWidget(PlayerController->GetPawn<APlayerCharacter>());

		PlayerController->bFinishGame = true;
		PlayerController->OnRep_FinishGame();
	}
}

void AInGameGS::ShowFinishWidget()
{
	APC* PlayerController = Cast<APC>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		PlayerController->ShowFinishWidget();
	}
}

void AInGameGS::UpdateMostAchievementValue(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount)
{
	if (MostEnableTriggerCount < InEnableTriggerCount)
	{
		MostEnableTriggerCount = InEnableTriggerCount;
	}

	if (MostCreateDecalCount < InMostCreateDecalCount)
	{
		MostCreateDecalCount = InMostCreateDecalCount;
	}

	if (MostEraseDecalCount < InMostEraseDecalCount)
	{
		MostEraseDecalCount = InMostEraseDecalCount;
	}

	if (MostOrganizePropsCount < InMostOrganizePropsCount)
	{
		MostOrganizePropsCount = InMostOrganizePropsCount;
	}

	if (MostSpillBucketCount < InMostSpillBucketCount)
	{
		MostSpillBucketCount = InMostSpillBucketCount;
	}

	if (MostProximityTime < InMostProximityTime)
	{
		MostProximityTime = InMostProximityTime;
	}

	if (MostBreakFurnitureCount < InMostBreakFurnitureCount)
	{
		MostBreakFurnitureCount = InMostBreakFurnitureCount;
	}

	if (MostStealCount < InMostStealCount)
	{
		MostStealCount = InMostStealCount;
	}

	if (MostBoxAndBucketSpawnCount < InMostSpawnBoxAndBucketCount)
	{
		MostBoxAndBucketSpawnCount = InMostSpawnBoxAndBucketCount;
	}
}

const FText AInGameGS::GetAchievementByUserID(FString InUserID)
{
	for (APlayerState* Temp : PlayerArray)
	{
		AInGamePS* PS = Cast<AInGamePS>(Temp);
		if (PS)
		{
			if (PS->UserID == InUserID && !PS->AchievementDataArr.IsEmpty())
			{
				return PS->AchievementDataArr[0].Script;
			}
		}
	}

	FAchievementData* Data = GetSpecialAchievementByID(TEXT("Default"));

	if (!Data)
		return FText::FromString(TEXT("Default Data isn't Exist"));

	return Data->Script;
}

void AInGameGS::UpdatePlayerState_Implementation()
{
	for (const auto& Temp : PlayerArray)
	{
		AInGamePS* PS = Cast<AInGamePS>(Temp);
		if (PS)
		{
			APC* PC = Cast<APC>(PS->GetPlayerController());
			if (PC)
			{
				PS->bIsInsideTruck = PC->bIsInTruck;
				PS->OnRep_UpdateInsideTruck();

				if (!PC->bIsInTruck)
				{
					if (PC->GetPawn())
					{
						APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
						if (Player->OwnedActor)
						{
							Player->DropObject();
						}
					}
				}
			}
		}
	}

	UpdateWholeAchievement();
}

void AInGameGS::SetTutorialTime(int32 InTime)
{
	TutorialTime = InTime;
	//UE_LOG(LogTemp, Warning, TEXT("TUTORIAL TIME : %d"), InTime);
}

void AInGameGS::S2A_DrawOutline_Implementation(ABasicInteractionObject* Object, bool DrawOutline)
{
	Object->DrawOutline(DrawOutline);
}

void AInGameGS::S2A_DecalDrawOutline_Implementation(AInteractionDecal* Object, bool DrawOutline)
{
	Object->HintDraw(DrawOutline);
}

void AInGameGS::EndGame_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("End Game"));
}

void AInGameGS::S2A_FinishGame_Implementation(class ACameraActor* Camera)
{
	APC* PlayerController = Cast<APC>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PlayerController)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("S2A_FinishGame"));

	APlayerCharacter* Player = PlayerController->GetPawn<APlayerCharacter>();

	if (!PlayerController->bIsInTruck)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Truck Out"));
		PlayerController->SetViewTargetWithBlend(Camera, 0.0f);  // 1 second blend

		if (Player)
		{
			Player->FPSMesh->SetVisibility(false);
			Player->GetMesh()->SetVisibility(true);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Truck In"));
	}

	EndGame();
}

void AInGameGS::UpdateWholeAchievement()
{
	//UE_LOG(LogTemp, Warning, TEXT("GS::UpdateAchievement"));

	if (PlayerArray.Num() < 2)
	{
		return;
	}

	for (int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		AInGamePS* PlayerStateA = Cast<AInGamePS>(PlayerArray[i]);

		if (PlayerStateA->AchievementDataArr.Num() == 0)
		{
			if (!PlayerStateA->AchievementDataArr.Contains(*GetSpecialAchievementByID(FName(TEXT("Default")))))
			{
				PlayerStateA->AchievementDataArr.Emplace(*GetSpecialAchievementByID(FName(TEXT("Default"))));
			}

			PlayerStateA->SortAchievement(PlayerStateA->AchievementDataArr);
			PlayerStateA->ForceNetUpdate();

			continue;
		}

		for (int32 j = i + 1; j < PlayerArray.Num(); ++j)
		{
			AInGamePS* PlayerStateB = Cast<AInGamePS>(PlayerArray[j]);
			if (PlayerStateB->AchievementDataArr.Num() == 0)
			{
				if (!PlayerStateB->AchievementDataArr.Contains(*GetSpecialAchievementByID(FName(TEXT("Default")))))
				{
					PlayerStateB->AchievementDataArr.Emplace(*GetSpecialAchievementByID(FName(TEXT("Default"))));
				}

				PlayerStateB->SortAchievement(PlayerStateB->AchievementDataArr);
				PlayerStateB->ForceNetUpdate();

				continue;
			}

			FAchievementData AchievementA;
			if (!PlayerStateA->AchievementDataArr.IsEmpty())
			{
				if (!((PlayerStateA->AchievementDataArr[0].Priority == 1) || (PlayerStateA->AchievementDataArr[0].Priority == 13)))
				{
					AchievementA = PlayerStateA->AchievementDataArr[0];
				}
			}

			FAchievementData AchievementB;
			if (!PlayerStateB->AchievementDataArr.IsEmpty())
			{
				if (!((PlayerStateB->AchievementDataArr[0].Priority == 1) || (PlayerStateB->AchievementDataArr[0].Priority == 13)))
				{
					AchievementB = PlayerStateB->AchievementDataArr[0];
				}
			}

			if ((AchievementA.Priority >= 100) && (AchievementB.Priority >= 100))
			{
				return;
			}

			if (AchievementA.Priority == AchievementB.Priority)
			{
				if (PlayerStateA->GetValueByPriority(AchievementA.Priority) == PlayerStateB->GetValueByPriority(AchievementB.Priority))
				{
					if (PlayerStateA)
					{
						PlayerStateA->AchievementDataArr.RemoveAt(0);
					}

					if (PlayerStateB)
					{
						PlayerStateB->AchievementDataArr.RemoveAt(0);
					}

					PlayerStateA->ForceNetUpdate();
					PlayerStateB->ForceNetUpdate();
				}
				else
				{
					if (PlayerStateA->GetValueByPriority(AchievementA.Priority) > PlayerStateB->GetValueByPriority(AchievementB.Priority))
					{
						if (PlayerStateB)
						{
							PlayerStateB->AchievementDataArr.RemoveAt(0);
						}

						PlayerStateB->ForceNetUpdate();
					}
					
					else if (PlayerStateA->GetValueByPriority(AchievementA.Priority) < PlayerStateB->GetValueByPriority(AchievementB.Priority))
					{
						if (PlayerStateA)
						{
							PlayerStateA->AchievementDataArr.RemoveAt(0);
						}

						PlayerStateA->ForceNetUpdate();
					}
				}
			}
		}
	}
}

void AInGameGS::SortAchievement(TArray<FAchievementData*>& InAchievements)
{
	Algo::Sort(InAchievements, [](FAchievementData* A, FAchievementData* B)
		{
			return A->Priority < B->Priority;
		});
}

void AInGameGS::UpdateFee()
{
	int32 DirtyNum = (InitTextureDecalCount - EraseTextureDecalNum + CreateTextureDecalCount) + (InitOpacityDecalCount - EraseOpacityDecalNum + CreateOpacityDecalCount) + TrashNum + WoodenPlankNum;

	if (DirtyNum <= Cleaning_1.Condition)
	{
		FinalCleaningFee += Cleaning_1.Fee;
	}
	if (DirtyNum >= Cleaning_2.Condition)
	{
		FinalCleaningFee += Cleaning_2.Fee;
	}
	if (DirtyNum <= Cleaning_3.Condition)
	{
		FinalCleaningFee += Cleaning_3.Fee;
	}

	if (CleanedPropsNum >= Additional.Condition)
	{
		FinalAdditionalFee += Additional.Fee;
	}

	int num = 0;
	for (APlayerState* Temp : PlayerArray)
	{
		AInGamePS* PS = Cast<AInGamePS>(Temp);
		if (PS)
		{
			if (!PS->bIsInsideTruck)
			{
				num++;
			}
		}
	}

	FinalTransportationFee = (num * TransportationFee);
}