// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CleanProgressBarBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UCleanProgressBarBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateCount(int32 InCount, int32 InitCount);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressBar")
	TObjectPtr<class UTextBlock> CountBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressBar")
	TObjectPtr<class UProgressBar> ProgressBarBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanProgressBar")
	TObjectPtr<class UTexture2D> IconTexture;
};
