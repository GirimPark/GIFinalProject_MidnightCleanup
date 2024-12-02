// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CleanAreaMonitorBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UCleanAreaMonitorBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanAreaMonitor")
	TObjectPtr<class UImage> FloorBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanAreaMonitor")
	TObjectPtr<class UButton> UpButtonBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CleanAreaMonitor")
	TObjectPtr<class UButton> DownButtonBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanAreaMonitor")
	TObjectPtr<class UTexture2D> FirstFloorTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanAreaMonitor")
	TObjectPtr<class UTexture2D> SecondFloorTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CleanAreaMonitor")
	TObjectPtr<class UMaterialInterface> FloorMaterialClass;

	UMaterialInstanceDynamic* FloorMaterial;

	UFUNCTION()
	void ClickedUpButton();

	UFUNCTION()
	void ClickedDownButton();

	bool IsHoveredButton();
};
