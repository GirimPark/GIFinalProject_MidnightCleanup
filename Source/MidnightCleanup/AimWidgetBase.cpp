// Fill out your copyright notice in the Description page of Project Settings.


#include "AimWidgetBase.h"
#include "Components/Image.h"

void UAimWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	IconType = EIconType::IconType_Hand;
	IconBox = Cast<UImage>(GetWidgetFromName(TEXT("AimIcon")));
}

void UAimWidgetBase::SetIconType(EIconType InType)
{
	if (!IconBox)
	{
		return;
	}

	if (IconType == InType)
	{
		return;
	}

	IconType = InType;
	
	switch (IconType)
	{
	case EIconType::IconType_Hand:
	{	
		if (!HandIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(HandIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Trash:
	{
		if (!TrashIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(TrashIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Wood:
	{
		if (!WoodIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(WoodIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Box:
	{
		if (!BoxIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(BoxIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Flashlight:
	{
		if (!FlashlightIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(FlashlightIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Mop:
	{
		if (!MopIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(MopIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Sponge:
	{
		if (!SpongeIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(SpongeIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Axe:
	{
		if (!AxeIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(AxeIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Bucket:
	{
		if (!BucketIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(BucketIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	case EIconType::IconType_Props:
	{
		if (!PropsIcon)
		{
			return;
		}

		FSlateBrush Brush = IconBox->GetBrush();
		Brush.SetResourceObject(PropsIcon);
		IconBox->SetBrush(Brush);
	}
	break;
	}
}
