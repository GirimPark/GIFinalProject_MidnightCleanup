// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicTool.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialSectionBase.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API URadialSectionBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetOpacity(float Value);
	void SetToolIconAngle(float Angle);
	void SetToolIconOpacity(float Value);
	void SetToolIconSize(float Value);
	void SetToolIcon(UTexture2D* InTexture, float InOpacity);
	void SetToolType(EToolType InToolType);
	EToolType GetToolType();
	
	UFUNCTION(BlueprintImplementableEvent)
	void IncreaseIconSizeAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void DecreaseIconSizeAnim();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RadialSection")
	TObjectPtr<class UImage> SectionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RadialSection")
	TObjectPtr<class UImage> ToolIconBox;

	UMaterialInstanceDynamic* SectionMaterial;

	UMaterialInstanceDynamic* ToolIconMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialSection")
	TObjectPtr<class UMaterialInterface> SectionMaterialClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialSection")
	TObjectPtr<class UMaterialInterface> ToolIconMaterialClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialSection")
	uint8 bIsHovered : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> HandIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> MopIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> FlashlightIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> SpongeIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialUI")
	TObjectPtr<class UTexture2D> AxeIconTexture;

	EToolType ToolType;
};
