// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FurnitureProgressBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UFurnitureProgressBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdatePercent(float InPercent);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FurnitureProgress")
	TObjectPtr<class UImage> FurnitureProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FurnitureProgress")
	TObjectPtr<class UMaterialInterface> ProgressMaterialClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FurnitureProgress")
	TObjectPtr<class UMaterialInstanceDynamic> ProgressMaterial;
};
