// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AchievementData.h"
#include "AimWidgetBase.h"
#include "PC.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API APC : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UInGameWidgetBase> InGameWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UInGameWidgetBase> InGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class URewardWidgetBase> RewardWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class URewardWidgetBase> RewardWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCleanProgressMonitorBase> CleanProgressWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UCleanProgressMonitorBase> CleanProgressWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPauseWidgetBase> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UPauseWidgetBase> PauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCaptionWidgetBase> CaptionWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UCaptionWidgetBase> CaptionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class URewardWidgetBase> FinishWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class URewardWidgetBase> FinishWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UFinishWorkProgressBarWidgetBase> FinishWorkProgressBarClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UFinishWorkProgressBarWidgetBase> FinishWorkProgressBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsInTruck : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 bEnableCaption : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPrimaryTrashBoxSpawner : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPrimaryTrashBoxTip : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPrimaryFacuetArea : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPrimaryWoodenPlankDisposalArea : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPrimaryToiletArea : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryWoodenPlank : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryProps : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryOldMop : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryOldSponge : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryStar_1 : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryStar_2 : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryStar_3 : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryStar_4 : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryStar_5 : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryCleanAllMopDecal : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryCleanAllSpongeDecal : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryCleanAllTrash : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bPrimaryCleanAllProps : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_FinishGame)
	uint8 bFinishGame : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bEnableModifyPlayTime : 1 = false;

	void ModifyPlayTime(bool State);

	void HideInGameWidget();

	void HideInventoryWidget(class APlayerCharacter* InPlayer);
	void ShowInventoryWidget(class APlayerCharacter* InPlayer);
	void UpdateInventoryByUI(class APlayerCharacter* InPlayer);
	bool IsEnableInventoryWidget();

	void HideAimWidget(class APlayerCharacter* InPlayer);
	void ShowAimWidget(class APlayerCharacter* InPlayer);
	void UpdateAimIcon(EIconType InType);

	UFUNCTION(BlueprintCallable)
	void ShowRewardWidget(class APlayerCharacter* InPlayer);
	void HideRewardWidget();
	bool IsEnableRewardWidget();

	void HideFurnitureProgressWidget();
	void ShowFurnitureProgressWidget();

	void HideCleanProgressWidget();
	void ShowCleanProgressWidget();
	void UpdateCleanProgressWidget();
	bool IsEnableCleanProgressWidget();

	void UpdateFurnitureProgressBar(float InMaxHp, float InCurrentHp);

	void UpdateToolTip(FText InText);
	void ShowToolTipWidget();
	void HideToolTipWidget();
	bool IsEnableToolTipWidget();

	void ShowPauseWidget();
	void HidePauseWidget();
	bool IsEnablePauseWidget();

	void UpdateCaption(FText InText, float Speed, const FString& InSFXPath);

	void ShowFinishWidget();

	void ShowFinishWorkProgressBarWidget();
	void HideFinishWorkProgressBarWidget();
	bool IsEnableFinishWorkProgressBarWidget();

	void ApplyPostProcessEffect(FPostProcessSettings PostProcessSetting, bool bEnableEffect);

	UFUNCTION(BlueprintCallable)
	void SetEnableCaption(bool State);

	UFUNCTION(BlueprintCallable)
	void SetTutorialTime(int32 InTime);

	UFUNCTION(BlueprintCallable)
	void OpenInitGame();
	
	UFUNCTION(Server, Reliable)
	void C2S_OpenInitGame();
	void C2S_OpenInitGame_Implementation();

	UFUNCTION(BlueprintCallable)
	bool IsCurrentWorldName(const FString& InName);

	UFUNCTION()
	void OnEndGameRespond();

	UFUNCTION()
	void OnRep_FinishGame();

	void EmplaceAchievementData(FAchievementData InData);
	void RemoveFirstAchievementData();
	void UpdateMostAchievementValue(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount);
	void UpdateOwnedAchievement();

	UFUNCTION(Server, Reliable)
	void C2S_UpdateMostAchievementValue(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount);
	void C2S_UpdateMostAchievementValue_Implementation(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_UpdateMostAchievementValue(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount);
	void S2A_UpdateMostAchievementValue_Implementation(int32 InEnableTriggerCount, int32 InMostCreateDecalCount, int32 InMostEraseDecalCount, int32 InMostOrganizePropsCount, int32 InMostSpillBucketCount, int32 InMostProximityTime, int32 InMostBreakFurnitureCount, int32 InMostStealCount, int32 InMostSpawnBoxAndBucketCount);

	UFUNCTION(Server, Reliable)
	void C2S_SetTutorialTime(int32 InTime);
	void C2S_SetTutorialTime_Implementation(int32 InTime);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetTutorialTime(int32 InTime);
	void S2A_SetTutorialTime_Implementation(int32 InTime);

	UFUNCTION(Server, Reliable)
	void C2S_SetEnableCaption(bool State);
	void C2S_SetEnableCaption_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetEnableCaption(bool State);
	void S2A_SetEnableCaption_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_UpdatePlayTime(bool State);
	void C2S_UpdatePlayTime_Implementation(bool State);
};