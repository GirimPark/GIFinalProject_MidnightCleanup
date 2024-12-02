// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardWidgetBase.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API URewardWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TObjectPtr<class UTextBlock> UserIDBox;
	TObjectPtr<class UButton> ReturnBox;
	TObjectPtr<class UTextBlock> CleaningRewardBox;
	TObjectPtr<class UTextBlock> AdditionalRewardBox;
	TObjectPtr<class UTextBlock> TransportationRewardBox;
	TObjectPtr<class UTextBlock> TotalRewardBox;
	TObjectPtr<class UTextBlock> TotalRewardBox_RightSide;
	TObjectPtr<class UHorizontalBox> StarLayoutBox;
	TObjectPtr<class UTextBlock> RewardScriptBox;
	TObjectPtr<class UGridPanel> CoworkerListBox;
	TArray<TObjectPtr<class AInGamePS>> CoworkerList;
	FText CoworkerAchievement;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> StarIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> StarBackgroundIconTexture;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void ClickedReturnButton();

	void UpdateReward();

	void UpdateStarIcon(int32 InValue);
	void UpdateTotalReward(int32 InValue);

	void UpdateCleaningFee(int32 InValue);
	void UpdateAdditionalFee(int32 InValue);
	void UpdateTransportationFee(int32 InValue);
	void UpdateRewardScript(const FText& InReward);
	void InitCoworkerList();
	void UpdateCoworkerList();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeInAnim();
};
