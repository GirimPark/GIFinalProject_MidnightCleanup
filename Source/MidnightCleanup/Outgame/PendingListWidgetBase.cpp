// Fill out your copyright notice in the Description page of Project Settings.


#include "PendingListWidgetBase.h"

#include "Components/TextBlock.h"

void UPendingListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UsernameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Username")));
}
