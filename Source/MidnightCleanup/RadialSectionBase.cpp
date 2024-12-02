// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialSectionBase.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PC.h"

void URadialSectionBase::NativeConstruct()
{
	Super::NativeConstruct();

	SectionBox = Cast<UImage>(GetWidgetFromName(TEXT("Section")));
	if (SectionBox)
	{
		SectionMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), SectionMaterialClass);
		if (SectionMaterial)
		{
			SectionMaterial->SetScalarParameterValue(FName("Opacity"), 0.f);
			SectionBox->SetBrushFromMaterial(SectionMaterial);
		}
	}

	ToolIconBox = Cast<UImage>(GetWidgetFromName(TEXT("ToolIcon")));
	if (ToolIconBox)
	{
		ToolIconMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ToolIconMaterialClass);
		if (ToolIconMaterial)
		{
			ToolIconMaterial->SetScalarParameterValue(FName("Opacity"), 0.f);
			ToolIconBox->SetBrushFromMaterial(ToolIconMaterial);
		}
	}
}

void URadialSectionBase::SetOpacity(float Value)
{
	if (SectionMaterial)
	{
		SectionMaterial->SetScalarParameterValue(FName("Opacity"), Value);
	}
	
	if (ToolIconMaterial)
	{
		ToolIconMaterial->SetScalarParameterValue(FName("Opacity"), Value);
	}	
}

void URadialSectionBase::SetToolIconAngle(float Angle)
{
	if (ToolIconMaterial)
	{
		ToolIconMaterial->SetScalarParameterValue(FName("Angle"), Angle);
	}
}

void URadialSectionBase::SetToolIconOpacity(float Value)
{
	if (ToolIconMaterial)
	{
		ToolIconMaterial->SetScalarParameterValue(FName("Opacity"), Value);
	}
}

void URadialSectionBase::SetToolIconSize(float Value)
{
	if (ToolIconBox)
	{
		FSlateBrush Brush = ToolIconBox->GetBrush();
		Brush.SetImageSize(FVector2D(Value, Value));
		ToolIconBox->SetBrush(Brush);
	}
}

void URadialSectionBase::SetToolIcon(UTexture2D* InTexture, float InOpacity)
{
	if (SectionMaterial)
	{
		ToolIconMaterial->SetTextureParameterValue(FName("Texture"), InTexture);
	}
	
	if (ToolIconMaterial)
	{
		ToolIconMaterial->SetScalarParameterValue(FName("Opacity"), InOpacity);
	}	
}

void URadialSectionBase::SetToolType(EToolType InToolType)
{
	ToolType = InToolType;

	switch (ToolType)
	{
	case EToolType::ToolType_Mop:
	{
		if (MopIconTexture)
		{
			SetToolIcon(MopIconTexture, 0.f);
		}
	}
	break;
	case EToolType::ToolType_Flashlight:
	{
		if (FlashlightIconTexture)
		{
			SetToolIcon(FlashlightIconTexture, 0.f);
		}
	}
	break;
	case EToolType::ToolType_Axe:
	{
		if (AxeIconTexture)
		{
			SetToolIcon(AxeIconTexture, 0.f);
		}
	}
	break;
	case EToolType::ToolType_Sponge:
	{
		if (SpongeIconTexture)
		{
			SetToolIcon(SpongeIconTexture, 0.f);
		}
	}
	break;
	case EToolType::ToolType_None:
	{
		if (HandIconTexture)
		{
			SetToolIcon(HandIconTexture, 0.f);
		}
	}
	break;
	}
}

EToolType URadialSectionBase::GetToolType()
{
	return ToolType;
}
