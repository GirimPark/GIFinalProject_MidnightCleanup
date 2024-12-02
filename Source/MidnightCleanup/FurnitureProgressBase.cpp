// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureProgressBase.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

void UFurnitureProgressBase::NativeConstruct()
{
	Super::NativeConstruct();

	FurnitureProgressBar = Cast<UImage>(GetWidgetFromName("Furniture_Progressbar"));

	if (ProgressMaterialClass)
	{
		ProgressMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ProgressMaterialClass);

		if (ProgressMaterial)
		{
			ProgressMaterial->SetScalarParameterValue(FName("Percent"), 1.f);

			if (FurnitureProgressBar)
			{
				FurnitureProgressBar->SetBrushFromMaterial(ProgressMaterial);
			}
		}
	}
}

void UFurnitureProgressBase::UpdatePercent(float InPercent)
{
	ProgressMaterial->SetScalarParameterValue(FName("Percent"), InPercent);
}
