// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketID.h"
#include "Blueprint/UserWidget.h"
#include "FriendListWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UFriendListWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetImageFromFriendState(EUserState UserState);

	void SetParentWidget(class UFriendManagementWidgetBase* Widget) { ParentWidget = Widget; }

	UFUNCTION()
	void OnDeleteFriendButtonClicked();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UTextBlock> UsernameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UImage> FriendStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UTextBlock> FriendStateText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	FLinearColor OnlineStateTintColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	FLinearColor InGameStateTintColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	FLinearColor OfflineStateTintColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> DeleteFriendButton;


private:
	class UFriendManagementWidgetBase* ParentWidget;
};
