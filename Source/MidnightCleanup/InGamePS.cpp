// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePS.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Seunggi/InGameGS.h"
#include "Kismet/GameplayStatics.h"
#include "PC.h"
#include "Seunggi/InGameGM.h"

AInGamePS::AInGamePS()
{
	EnableTriggerCount = 0;
	CreateDecalCount = 0;
	EraseDecalCount = 0;
	OrganizePropsCount = 0;
	BreakFurnitureCount = 0;
	StealCount = 0;
	SpawnBoxAndBucketCount = 0;
	SpillBucketCount = 0;
	TotalProximityTime = 0;
	bIsInsideTruck = true;
	ThumbsUpNum = 0;
	UserID = TEXT("None");
}

void AInGamePS::OnRep_UpdateReward()
{
	UpdateMostAchievementValue();
	UpdateAchievement();
}

void AInGamePS::OnRep_UpdateInsideTruck()
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC && !PC->bIsInTruck)
	{
		PC->EmplaceAchievementData(*GS->GetSpecialAchievementByID(FName(TEXT("Loser"))));
	}
}

void AInGamePS::UpdateAchievement()
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->UpdateOwnedAchievement();
	}
}

void AInGamePS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGamePS, EnableTriggerCount);
	DOREPLIFETIME(AInGamePS, CreateDecalCount);
	DOREPLIFETIME(AInGamePS, EraseDecalCount);
	DOREPLIFETIME(AInGamePS, OrganizePropsCount);
	DOREPLIFETIME(AInGamePS, BreakFurnitureCount);
	DOREPLIFETIME(AInGamePS, StealCount);
	DOREPLIFETIME(AInGamePS, SpawnBoxAndBucketCount);
	DOREPLIFETIME(AInGamePS, SpillBucketCount);
	DOREPLIFETIME(AInGamePS, TotalProximityTime);
	DOREPLIFETIME(AInGamePS, PlayerCharacter);
	DOREPLIFETIME(AInGamePS, bIsInsideTruck);
	DOREPLIFETIME(AInGamePS, UserID);
	DOREPLIFETIME(AInGamePS, ThumbsUpNum);
	DOREPLIFETIME(AInGamePS, AchievementDataArr);
	DOREPLIFETIME(AInGamePS, LastBeginOverlapTime);
	DOREPLIFETIME(AInGamePS, ProximityTimes);
}

void AInGamePS::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProximityTimes.Empty();
}

void AInGamePS::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("Property copy"));
	if (PlayerState != nullptr)
	{
		AInGamePS* IngamePS = Cast<AInGamePS>(PlayerState);
		if (IngamePS)
		{
			IngamePS->PlayerCharacter = PlayerCharacter;
		}
	}
}

void AInGamePS::SortAchievement(TArray<FAchievementData>& InAchievements)
{
	Algo::Sort(InAchievements, [](FAchievementData A, FAchievementData B)
		{
			return A.Priority < B.Priority;
		});
}

int32 AInGamePS::GetValueByPriority(int32 Priority)
{
	switch (Priority)
	{
	case 2:
	{
		return EnableTriggerCount;
	}
	break;
	case 3:
	{
		return TotalProximityTime;
	}
	break;
	case 5:
	{
		return SpillBucketCount;
	}
	break;
	case 6:
	{
		return CreateDecalCount;
	}
	break;
	case 8:
	{
		return EraseDecalCount;
	}
	break;
	case 9:
	{
		return OrganizePropsCount;
	}
	break;
	case 10:
	{
		return BreakFurnitureCount;
	}
	break;
	case 11:
	{
		return StealCount;
	}
	break;
	case 12:
	{
		return SpawnBoxAndBucketCount;
	}
	break;
	}

	return -1;
}

void AInGamePS::UpdateMostAchievementValue()
{
	// Update GameState Most Achievement Value
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->UpdateMostAchievementValue(EnableTriggerCount, CreateDecalCount, EraseDecalCount, OrganizePropsCount, SpillBucketCount, TotalProximityTime, BreakFurnitureCount, StealCount, SpawnBoxAndBucketCount);
	}
}

void AInGamePS::C2S_UpdateAchievement_Implementation()
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	if (!GS->RankedAchievements.IsEmpty())
	{
		for (int i = GS->RankedAchievements.Num() - 1; i >= 0; i--)
		{
			if (GetValueByPriority(GS->RankedAchievements[i]->Priority) >= GS->RankedAchievements[i]->Minimum)
			{
				S2A_EmplaceAchievementData(*GS->RankedAchievements[i]);
			}
		}
	}

	if (!AchievementDataArr.IsEmpty())
	{

		switch (AchievementDataArr[0].Priority)
		{
		case 2:
		{
			if (!(EnableTriggerCount >= GS->MostEnableTriggerCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 3:
		{
			if (!(TotalProximityTime >= GS->MostProximityTime))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 5:
		{
			if (!(SpillBucketCount >= GS->MostSpillBucketCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 6:
		{
			if (!(CreateDecalCount >= GS->MostCreateDecalCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 8:
		{
			if (!(EraseDecalCount >= GS->MostEraseDecalCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 9:
		{
			if (!(OrganizePropsCount >= GS->MostOrganizePropsCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 10:
		{
			if (!(BreakFurnitureCount >= GS->MostBreakFurnitureCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 11:
		{
			if (!(StealCount >= GS->MostStealCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		case 12:
		{
			if (!(SpawnBoxAndBucketCount >= GS->MostBoxAndBucketSpawnCount))
			{
				S2A_RemoveFirstAchievementData();
			}
		}
		break;
		}

		if (AchievementDataArr.IsEmpty())
		{
			return;
		}
		 
		if (AchievementDataArr[0].Priority > 4)
		{
			if ((CreateDecalCount >= GS->GetAchievementByID(FName(TEXT("DecalMaker")))->Minimum) && (SpillBucketCount >= GS->GetAchievementByID(FName(TEXT("BucketBreaker")))->Minimum))
			{
				if (GS->MostCreateDecalCount == CreateDecalCount && GS->MostSpillBucketCount == SpillBucketCount)
				{
					S2A_EmplaceAchievementData(*GS->GetSpecialAchievementByID(FName(TEXT("TroubleMaker"))));
				}
			}
		}

		if (AchievementDataArr[0].Priority > 7)
		{
			if ((EraseDecalCount >= GS->GetAchievementByID(FName(TEXT("CleaningMaster")))->Minimum) && (OrganizePropsCount >= GS->GetAchievementByID(FName(TEXT("OrganizationMaster")))->Minimum))
			{
				if (GS->MostEraseDecalCount == EraseDecalCount && GS->MostOrganizePropsCount == OrganizePropsCount)
				{
					S2A_EmplaceAchievementData(*GS->GetSpecialAchievementByID(FName(TEXT("Elite"))));
				}
			}
		}
	}
}

void AInGamePS::C2S_EmplaceAchievementData_Implementation(FAchievementData InData)
{
	S2A_EmplaceAchievementData(InData);
}

void AInGamePS::S2A_EmplaceAchievementData_Implementation(FAchievementData InData)
{
	if (!AchievementDataArr.Contains(InData))
	{
		AchievementDataArr.Emplace(InData);
	}

	SortAchievement(AchievementDataArr);
}

void AInGamePS::S2A_RemoveFirstAchievementData_Implementation()
{
	AchievementDataArr.RemoveAt(0);
}

void AInGamePS::UpdateLastOverlapTime(const FString& InUserID, int32 InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Proximity : BeginOverlapTime : %d"), InValue);
	if (!ProximityTimes.Contains(FPlayerProximityData(InUserID, InValue, 0)))
	{
		ProximityTimes.Emplace(FPlayerProximityData(InUserID, InValue, 0));
	}
	else
	{
		for (int i = 0; i < ProximityTimes.Num(); i++)
		{
			if (InUserID == ProximityTimes[i].UserID && ProximityTimes[i].LastBeginOverlapTime >= InValue)
			{
				ProximityTimes[i].LastBeginOverlapTime = InValue;
			}
		}
	}
}

void AInGamePS::UpdateProximityDuration(const FString& InUserID, int32 InValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Proximity : EndOverlapTime : %d"), InValue);
	if (ProximityTimes.Contains(FPlayerProximityData(InUserID, InValue, 0)))
	{
		for (int i = 0; i < ProximityTimes.Num(); i++)
		{
			if (InUserID == ProximityTimes[i].UserID && ProximityTimes[i].LastBeginOverlapTime >= InValue)
			{
				ProximityTimes[i].OverlappingTime += (ProximityTimes[i].LastBeginOverlapTime - InValue);
			}
		}
	}

	int32 Temp = 0;
	for (int i = 0; i < ProximityTimes.Num(); i++)
	{
		Temp += ProximityTimes[i].OverlappingTime;
	}

	TotalProximityTime = Temp;
	UE_LOG(LogTemp, Warning, TEXT("[%s] : %d"), *UserID, TotalProximityTime);

	OnRep_UpdateReward();
}
