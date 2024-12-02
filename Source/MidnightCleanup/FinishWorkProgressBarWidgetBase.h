// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinishWorkProgressBarWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UFinishWorkProgressBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TObjectPtr<class UImage> ProgressBarBox;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FinishWorkProgress")
	TObjectPtr<class UMaterialInterface> ProgressMaterialClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FinishWorkProgress")
	TObjectPtr<class UMaterialInstanceDynamic> ProgressMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FinishWorkProgress")
	float LeftClickTime;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool IsEnableProgressBar();
	void ShowProgressBar();
	void HideProgressBar();

	UFUNCTION(BlueprintCallable)
	void UpdateProgressBar(float InRatio, APlayerCharacter* InPlayer);
};
