// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ULoginWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnLoginRespond(int32 response);

	UFUNCTION()
	void OnLoginActionTriggered();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnLoginButtonClicked();

	UFUNCTION()
	void OnSignupButtonClicked();

	UFUNCTION(BlueprintCallable)
	void ResetLoginPopup();

	UFUNCTION(BlueprintCallable)
	void ResetJoinPopup();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat");
	FString IpAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TSubclassOf<class UInvitationAlertWidgetBase> InvitationAlertWidgetClass;

private:
	TObjectPtr<class UButton> LoginButton;
	TObjectPtr<class UEditableTextBox> SigninIDText;
	TObjectPtr<class UEditableTextBox> SigninPWText;
	TObjectPtr<class UTextBlock> LoginFailAlertText;

	TObjectPtr<class UButton> JoinButton;
	TObjectPtr<class UEditableTextBox> SignupIDText;
	TObjectPtr<class UEditableTextBox> SignupPWText;
	TObjectPtr<class UEditableTextBox> SignupPWCheckText;
	TObjectPtr<class UTextBlock> JoinIDResText;
	TObjectPtr<class UTextBlock> JoinPWResText;
};
