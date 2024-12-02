// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialUIBase.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API URadialUIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetToolDescription(UTexture2D* InTexture, float InOpacity);
	void SetOpacity(float Value);
	void SetMousePos(int Index);
	void UpdateInventory(float Degree);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RaidalUI")
	TObjectPtr<class UImage> ArrowBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RaidalUI")
	TObjectPtr<class UImage> DescriptionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RaidalUI")
	TObjectPtr<class UMaterialInterface> ArrowMaterialClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RaidalUI")
	TObjectPtr<class UMaterialInterface> ToolDescriptionMaterialClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RaidalUI")
	int32 CurrentInventoryIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> HandDescriptionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> MopDescriptionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> FlashlightDescriptionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> SpongeDescriptionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> AxeDescriptionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	float SmallIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	float BigIconSize;

	UMaterialInstanceDynamic* ArrowMaterial;

	UMaterialInstanceDynamic* ToolDescriptionMaterial;

	TArray<class URadialSectionBase*> Inventory;
};
