// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CleanProgressMonitorBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UCleanProgressMonitorBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UCleanProgressBarBase> MopDecalProgressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UCleanProgressBarBase> SpongeDecalProgressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UCleanProgressBarBase> FurnitureProgressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UCleanProgressBarBase> TrashProgressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UCleanProgressBarBase> WoodProgressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class UCleanProgressBarBase> PropsProgressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanProgressMonitor")
	TObjectPtr<class ULimitTimeBase> LimitTimeBox;

	TObjectPtr<class UHorizontalBox> StarLayoutBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> StarIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> StarBackgroundIconTexture;

	UFUNCTION()
	void UpdateCount(int32 InMopDecal, int32 InSpongeDecal, int32 InFurniture, int32 InTrash, int32 InWood, int32 InProps, int32 InitMopDecal, int32 InitSpongeDecal, int32 InitFurniture, int32 InitTrash, int32 InitWood, int32 InitProps, int32 InCreateTextureDecal);

	void UpdatePlayTime(int32 InSeconds);

	void UpdateStarIcon(int32 InValue);
};
