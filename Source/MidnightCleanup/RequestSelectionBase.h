// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestSelectionBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API URequestSelectionBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UButton> StageBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UButton> StageBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UButton> StageBox3;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UImage> ProgressBarImage1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UImage> ProgressBarImage2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UImage> ProgressBarImage3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RequestSelection")
	TObjectPtr<class UMaterialInterface> ProgressMaterialClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RequestSelection")
	TObjectPtr<class UMaterialInstanceDynamic> ProgressMaterial;



	float ButtonFirstPressedTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RequestSelection")
	float SelectMapTime = 1.f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UButton> ReturnBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UTextBlock> UserIDBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RequestSelection")
	TObjectPtr<class UTextBlock> CashBox;


	void UpdateProgressBar(TObjectPtr<UImage>& ProgressBarImage, float InRatio);

	UFUNCTION()
	void OnStage1ButtonPressed();

	UFUNCTION()
	void OnStage1ButtonReleased();

	UFUNCTION()
	void OnStage2ButtonPressed();

	UFUNCTION()
	void OnStage2ButtonReleased();

	UFUNCTION()
	void OnStage3ButtonPressed();

	UFUNCTION()
	void OnStage3ButtonReleased();


	UFUNCTION()
	void OnStartGameRespond();

	UFUNCTION()
	void ClickedReturnButton();

	void OpenInGame();

	void EnableLoadingScreen();

private:
	uint8 bButton1Pressed : 1 = false;
	uint8 bButton2Pressed : 1 = false;
	uint8 bButton3Pressed : 1 = false;
};
