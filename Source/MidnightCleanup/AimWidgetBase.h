// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimWidgetBase.generated.h"

UENUM(BlueprintType)
enum class EIconType : uint8
{
	IconType_Hand = 0 UMETA(DisplayName = "Hand"),
	IconType_Trash = 1 UMETA(DisplayName = "Trash"),
	IconType_Wood = 2 UMETA(DisplayName = "Wood"),
	IconType_Box = 3 UMETA(DisplayName = "Box"),
	IconType_Flashlight = 4 UMETA(DisplayName = "Flashlight"),
	IconType_Mop = 5 UMETA(DisplayName = "Mop"),
	IconType_Sponge = 6 UMETA(DisplayName = "Sponge"),
	IconType_Axe = 7 UMETA(DisplayName = "Axe"),
	IconType_Bucket = 8 UMETA(DisplayName = "Bucket"),
	IconType_Props = 9 UMETA(DisplayName = "Props")
};

UCLASS()
class MIDNIGHTCLEANUP_API UAimWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> HandIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> TrashIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> WoodIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> BoxIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> FlashlightIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> MopIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> SpongeIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> AxeIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> BucketIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> PropsIcon;

	void SetIconType(EIconType InType);

private:
	EIconType IconType;
	TObjectPtr<class UImage> IconBox;
};
