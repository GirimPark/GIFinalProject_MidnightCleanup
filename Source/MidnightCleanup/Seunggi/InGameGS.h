// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../AchievementData.h"
#include "../ToolTipData.h"
#include "../CaptionData.h"
#include "InGameGS.generated.h"

USTRUCT(BlueprintType)
struct FStarRange
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	int32 MinValue = 0;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	int32 MaxValue = 0;
};


USTRUCT(BlueprintType)
struct FConditionWithFee
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "-20000.0", UIMin = "0.0"))
	int32 Fee = 0;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	int32 Condition = 0;
};

UCLASS()
class MIDNIGHTCLEANUP_API AInGameGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 TrashNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 EraseTextureDecalNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 EraseOpacityDecalNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint32 WoodenPlankNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint8 BreakFurnitureNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	uint8 CleanedPropsNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitFurnitureCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitTextureDecalCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float CreateTextureDecalCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float CreateOpacityDecalCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitOpacityDecalCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitTrashCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitWoodCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	float InitPropsCount = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	int32 BucketNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCount)
	int32 BoxNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateTime)
	int32 OriginPlayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_UpdateTime)
	int32 PlayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateTime)
	int32 CurrentPlayTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FName> TimeMileStones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConditionWithFee Cleaning_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConditionWithFee Cleaning_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConditionWithFee Cleaning_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConditionWithFee Additional;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TransportationFee;

	UPROPERTY(Replicated)
	int32 FinalCleaningFee = 0;

	UPROPERTY(Replicated)
	int32 FinalAdditionalFee = 0;

	UPROPERTY(Replicated)
	int32 FinalTransportationFee = 0;

	UPROPERTY(Replicated)
	int32 StarNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarCondition_0 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarRange StarCondition_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarRange StarCondition_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarRange StarCondition_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarRange StarCondition_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarCondition_5 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> RewardScripts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> AchievementDataTable;

	FAchievementData* GetAchievementByID(FName ID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> SpecialAchievementDataTable;

	FAchievementData* GetSpecialAchievementByID(FName ID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> ToolTipDataTable;

	FToolTipData* GetToolTipByID(FName ID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> CaptionDataTable;

	FCaptionData* GetCaptionByID(FName ID);

	// Achievement data table (sort by priority)
	TArray<FAchievementData*> RankedAchievements;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostCreateDecalCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostEraseDecalCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostOrganizePropsCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostSpillBucketCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostProximityTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostEnableTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostBreakFurnitureCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostStealCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 MostBoxAndBucketSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 TutorialTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float LoadingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float PressKeypadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ModifyTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bPrimaryModifyPlayTime : 1 = false;

	UPROPERTY(Replicated)
	int32 PlayerNum;

	UFUNCTION()
	void OnRep_UpdateCount();

	UFUNCTION()
	void OnRep_UpdateTime();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay()override;

	void UpdateFee();
	void UpdateStar();

	UFUNCTION(BlueprintNativeEvent)
	void EndGame();
	void EndGame_Implementation();

	UFUNCTION(NetMulticast,Reliable)
	void S2A_FinishGame(class ACameraActor* Camera);
	void S2A_FinishGame_Implementation(class ACameraActor* Camera);

	void UpdateWholeAchievement();

	void SortAchievement(TArray<FAchievementData*>& InAchievements);

	UFUNCTION(BlueprintCallable)
	void ShowRewardWidget();

	UFUNCTION(BlueprintCallable)
	void ShowFinishWidget();

	void UpdateMostAchievementValue(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount);

	const FText GetAchievementByUserID(FString InUserID);

	UFUNCTION(NetMulticast, Reliable)
	void UpdatePlayerState();
	void UpdatePlayerState_Implementation();

	void SetTutorialTime(int32 InTime);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_DrawOutline(ABasicInteractionObject* Object, bool DrawOutline);
	void S2A_DrawOutline_Implementation(ABasicInteractionObject* Object, bool DrawOutline);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_DecalDrawOutline(AInteractionDecal* Object, bool DrawOutline);
	void S2A_DecalDrawOutline_Implementation(AInteractionDecal* Object, bool DrawOutline);
};
