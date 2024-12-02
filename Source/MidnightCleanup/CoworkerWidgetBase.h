// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoworkerWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UCoworkerWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TObjectPtr<class UTextBlock> GradeBox;
	TObjectPtr<class UTextBlock> UserIDBox;
	TObjectPtr<class UTextBlock> AchievementBox;
	TObjectPtr<class UButton> ThumbsUpButton;
	TObjectPtr<class UHorizontalBox> ThumbsUpLayoutBox;
	TObjectPtr<class UImage> ThumbsUpDisableBox;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> ThumbsUpIconTexture;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetGradeBox(FText InValue);
	void SetUserIDBox(FText InValue);
	void SetAchievementBox(FText InValue);
	void UpdateThumbsUpIcon(int32 InValue);
	void HideThumbsUpButton();

	UFUNCTION()
	void ClickedThumbsUpButton();
};
