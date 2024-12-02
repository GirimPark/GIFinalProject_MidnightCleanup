// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AchievementData.h"
#include "InGamePS.generated.h"

USTRUCT(BlueprintType)
struct FPlayerProximityData
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserID;

	UPROPERTY()
	int32 LastBeginOverlapTime;

	UPROPERTY()
	int32 OverlappingTime;

	FPlayerProximityData()
		: UserID(TEXT("None")), LastBeginOverlapTime(0), OverlappingTime(0)
	{
	}

	FPlayerProximityData(const FString& InString, int32 InLastOverlapTime, int32 InOverlappingTime)
		: UserID(InString)
		, LastBeginOverlapTime(InLastOverlapTime)
		, OverlappingTime(InOverlappingTime)
	{
	}

	bool operator==(const FPlayerProximityData& Other) const
	{
		return UserID == Other.UserID;
	}
};

UCLASS()
class MIDNIGHTCLEANUP_API AInGamePS : public APlayerState
{
	GENERATED_BODY()
	
public:
	AInGamePS();

	UFUNCTION()
	void OnRep_UpdateReward();

	UFUNCTION()
	void OnRep_UpdateInsideTruck();

	void UpdateAchievement();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FAchievementData> AchievementDataArr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 EnableTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 CreateDecalCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 EraseDecalCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 OrganizePropsCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 BreakFurnitureCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 StealCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 SpawnBoxAndBucketCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 SpillBucketCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 TotalProximityTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FPlayerProximityData> ProximityTimes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Replicated)
	TSubclassOf<class APlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateInsideTruck)
	uint8 bIsInsideTruck : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateReward)
	int32 LastBeginOverlapTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FString UserID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 ThumbsUpNum;

	virtual void PostInitializeComponents() override;

	virtual void CopyProperties(class APlayerState* PlayerState) override;

	void SortAchievement(TArray<FAchievementData>& InAchievements);

	int32 GetValueByPriority(int32 Priority);

	void UpdateMostAchievementValue();

	UFUNCTION(Server, Reliable)
	void C2S_UpdateAchievement();
	void C2S_UpdateAchievement_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_EmplaceAchievementData(FAchievementData InData);
	void C2S_EmplaceAchievementData_Implementation(FAchievementData InData);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_EmplaceAchievementData(FAchievementData InData);
	void S2A_EmplaceAchievementData_Implementation(FAchievementData InData);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_RemoveFirstAchievementData();
	void S2A_RemoveFirstAchievementData_Implementation();

	void UpdateLastOverlapTime(const FString& InUserID, int32 InValue);

	void UpdateProximityDuration(const FString& InUserID, int32 InValue);
};
