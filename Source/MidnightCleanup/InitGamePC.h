// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InitGamePC.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInitGamePC : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowLaptopWidget();
	void HideLaptopWidget();
	void ShowToolTipWidget();
	void HideToolTipWidget();
	void UpdateToolTip(FText InText);
	void ShowPauseWidget();
	void HidePauseWidget();
	bool IsEnablePauseWidget();
	bool IsEnableLaptopWidget();
	void ShowLoadingWidget();
	void SetOpenLevelName(const FString& InName);
	void OpenInGame();

	UFUNCTION(BlueprintCallable)
	void EnableLoadingScreen();

	UFUNCTION(Server, Reliable)
	void C2S_EnableLoadingScreen(APlayerCharacter* InPlayer);
	void C2S_EnableLoadingScreen_Implementation(APlayerCharacter* InPlayer);
	
	UFUNCTION(Server, Reliable)
	void C2S_OpenInGame();
	void C2S_OpenInGame_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_SetOpenLevelName(const FString& InLevelName);
	void C2S_SetOpenLevelName_Implementation(const FString& InLevelName);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetOpenLevelName(const FString& InLevelName);
	void S2A_SetOpenLevelName_Implementation(const FString& InLevelName);

	UFUNCTION()
	void OnJoinRoomRespond(int32 ResultCode, FString IpAddress);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class URoomCodeWidgetBase> RoomCodeWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class URoomCodeWidgetBase> RoomCodeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ULaptopWidgetBase> LaptopWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULaptopWidgetBase> LaptopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UToolTipWidgetBase> ToolTipWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UToolTipWidgetBase> ToolTipWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> AimWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPauseWidgetBase> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UPauseWidgetBase> PauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UInvitationAlertWidgetBase> InvitationAlertWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UInvitationAlertWidgetBase> InvitationAlertWidget;

	UPROPERTY(Replicated)
	FString OpenLevelName;
};
