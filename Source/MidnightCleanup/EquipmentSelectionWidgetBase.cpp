// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSelectionWidgetBase.h"

#include "GI.h"
#include "InitGamePC.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UEquipmentSelectionWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	NicknameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Nickname")));
	CashText = Cast<UTextBlock>(GetWidgetFromName("Text_Cash"));
	ReturnButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Return")));

	if(!(NicknameText && CashText && ReturnButton))
	{
		UE_LOG(LogTemp, Error, TEXT("GetWidgetFromName Failed(UEquipmentSelectionWidgetBase::NativeConstruct)"));
		return;
	}

	UGI* GI = Cast<UGI>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast To GI Failed(UEquipmentSelectionWidgetBase::NativeConstruct)"));
		return;
	}
	
	NicknameText->SetText(FText::FromString(GI->GetUserID()));

	FString CashStr = FString::FromInt(GI->Cash) + TEXT("\uFFE5");
	CashText->SetText(FText::FromString(CashStr));

	ReturnButton->OnClicked.AddDynamic(this, &UEquipmentSelectionWidgetBase::OnReturnButtonClicked);
}

void UEquipmentSelectionWidgetBase::OnReturnButtonClicked()
{
	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to InitGamePC Failed(UEquipmentSelectionWidgetBase::OnReturnButtonClicked)"));
		return;
	}

	PC->HideLaptopWidget();
}
