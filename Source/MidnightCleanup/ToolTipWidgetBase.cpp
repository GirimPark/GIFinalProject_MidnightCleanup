// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolTipWidgetBase.h"
#include "Components/EditableTextBox.h"

void UToolTipWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ScriptBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ToopTipText")));
}

void UToolTipWidgetBase::UpdateScript(FText InText)
{
	if (ScriptBox)
	{
		ScriptBox->SetText(InText);
	}
}

void UToolTipWidgetBase::SetOpacity(float InValue)
{
	if (ScriptBox)
	{
		ScriptBox->SetRenderOpacity(InValue);
	}
}
