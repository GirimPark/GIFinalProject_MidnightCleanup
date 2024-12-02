// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanAreaMonitorBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PC.h"
#include "PlayerCharacter.h"

void UCleanAreaMonitorBase::NativeConstruct()
{
	Super::NativeConstruct();

	UpButtonBox = Cast<UButton>(GetWidgetFromName(TEXT("UpBTN")));
	if (UpButtonBox)
	{
		UpButtonBox->OnPressed.AddDynamic(this, &UCleanAreaMonitorBase::ClickedUpButton);
	}
	DownButtonBox = Cast<UButton>(GetWidgetFromName(TEXT("DownBTN")));
	if (DownButtonBox)
	{
		DownButtonBox->OnPressed.AddDynamic(this, &UCleanAreaMonitorBase::ClickedDownButton);
	}

	FloorMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), FloorMaterialClass);
	if (FloorMaterial && FirstFloorTexture)
	{
		FloorMaterial->SetTextureParameterValue(FName("Texture"), FirstFloorTexture);
	}

	FloorBox = Cast<UImage>(GetWidgetFromName(TEXT("FloorPlan")));
	if (FloorBox)
	{
		FloorBox->SetBrushFromMaterial(FloorMaterial);
	}
}

void UCleanAreaMonitorBase::ClickedUpButton()
{
	if (FloorMaterial && SecondFloorTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClickedUpButton"));
		FloorMaterial->SetTextureParameterValue(FName("Texture"), SecondFloorTexture);
	}
}

void UCleanAreaMonitorBase::ClickedDownButton()
{
	if (FloorMaterial && FirstFloorTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClickedDownButton"));
		FloorMaterial->SetTextureParameterValue(FName("Texture"), FirstFloorTexture);
	}
}

bool UCleanAreaMonitorBase::IsHoveredButton()
{
	if (UpButtonBox->IsHovered() || DownButtonBox->IsHovered())
	{
		UE_LOG(LogTemp, Warning, TEXT("RETURN TRUE"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RETURN FALSE"));
		return false;
	}
}


