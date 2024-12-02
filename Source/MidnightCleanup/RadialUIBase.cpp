// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialUIBase.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "RadialSectionBase.h"
#include "PlayerCharacter.h"
#include "BasicTool.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URadialUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentInventoryIndex = 0;

	Inventory.SetNum(4);
	Inventory[0] = Cast<URadialSectionBase>(GetWidgetFromName(TEXT("Slot0")));
	if (Inventory[0])
	{
		Inventory[0]->SetToolIconAngle(0.f);
	}
	Inventory[1] = Cast<URadialSectionBase>(GetWidgetFromName(TEXT("Slot1")));
	if (Inventory[1])
	{
		Inventory[1]->SetToolIconAngle(270.f);
	}
	Inventory[2] = Cast<URadialSectionBase>(GetWidgetFromName(TEXT("Slot2")));
	if (Inventory[2])
	{
		Inventory[2]->SetToolIconAngle(180.f);
	}
	Inventory[3] = Cast<URadialSectionBase>(GetWidgetFromName(TEXT("Slot3")));
	if (Inventory[3])
	{
		Inventory[3]->SetToolIconAngle(90.f);
	}

	for (URadialSectionBase* Temp : Inventory)
	{
		if (Temp)
		{
			Temp->SetOpacity(0.f);
			Temp->SetToolIconSize(100.f);
		}		
	}

	DescriptionBox = Cast<UImage>(GetWidgetFromName(TEXT("ToolDescription")));
	if (DescriptionBox)
	{
		ToolDescriptionMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ToolDescriptionMaterialClass);
		if (ToolDescriptionMaterial)
		{
			DescriptionBox->SetBrushFromMaterial(ToolDescriptionMaterial);
		}
	}

	ArrowBox = Cast<UImage>(GetWidgetFromName(TEXT("CenterArrow")));
	if (ArrowBox)
	{
		ArrowMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ArrowMaterialClass);
		if (ArrowMaterial)
		{
			ArrowBox->SetBrushFromMaterial(ArrowMaterial);
		}
	}
}

void URadialUIBase::SetToolDescription(UTexture2D* InTexture, float InOpacity)
{
	if (ToolDescriptionMaterial)
	{
		ToolDescriptionMaterial->SetTextureParameterValue(FName("Texture"), InTexture);
		ToolDescriptionMaterial->SetScalarParameterValue(FName("Opacity"), InOpacity);
	}
}

void URadialUIBase::SetOpacity(float Value)
{
	if (ToolDescriptionMaterial)
	{
		ToolDescriptionMaterial->SetScalarParameterValue(FName("Opacity"), Value);
	}
}

void URadialUIBase::SetMousePos(int Index)
{
	int32 ScreenWidth =0;
	int32 ScreenHeight = 0;

	FVector2D MousePosition;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->GetViewportSize(ScreenWidth, ScreenHeight);
		PC->GetMousePosition(MousePosition.X, MousePosition.Y);
	}

	FVector2D TopLeft(0.0f, 0.0f);
	FVector2D TopRight(ScreenWidth, 0.0f);
	FVector2D BottomRight(ScreenWidth, ScreenHeight);
	FVector2D BottomLeft(0.0f, ScreenHeight);
	FVector2D Center(ScreenWidth / 2, ScreenHeight / 2);

	switch (Index)
	{
	case 0:
	{
		FVector2D NewMousePos((TopLeft.X + Center.X + TopRight.X) / 3.0f, (TopLeft.Y + Center.Y + TopRight.Y) / 3.0f);
		PC->SetMouseLocation(NewMousePos.X, NewMousePos.Y);
	}
	break;
	case 1:
	{
		FVector2D NewMousePos((TopRight.X + Center.X + BottomRight.X) / 3.0f, (TopRight.Y + Center.Y + BottomRight.Y) / 3.0f);
		PC->SetMouseLocation(NewMousePos.X, NewMousePos.Y);
	}
	break;
	case 2:
	{
		FVector2D NewMousePos((BottomRight.X + Center.X + BottomLeft.X) / 3.0f, (BottomRight.Y + Center.Y + BottomLeft.Y) / 3.0f);
		PC->SetMouseLocation(NewMousePos.X, NewMousePos.Y);
	}
	break;
	case 3:
	{
		FVector2D NewMousePos((BottomLeft.X + Center.X + TopLeft.X) / 3.0f, (BottomLeft.Y + Center.Y + TopLeft.Y) / 3.0f);
		PC->SetMouseLocation(NewMousePos.X, NewMousePos.Y);
	}
	break;
	}
}

void URadialUIBase::UpdateInventory(float Degree)
{
	int32 Index = 0;

	float Temp = 360.f / Inventory.Num();

	if (Degree > 0)
	{
		Degree = abs(Degree);

		if (Degree > 0 && Degree <= Temp / 2)
		{
			Index = 0;
		}

		else if (Degree > Temp / 2 && Degree <= Temp / 2 + Temp)
		{
			Index = 1;
		}

		else if (Degree > Temp / 2 + Temp && Degree <= Temp * 2)
		{
			Index = 2;
		}
	}
	else if(Degree < 0)
	{
		Degree = abs(Degree);

		if (Degree > 0 && Degree <= Temp / 2)
		{
			Index = 0;
		}

		else if (Degree > Temp / 2 && Degree <= Temp / 2 + Temp)
		{
			Index = 3;
		}

		else if (Degree > Temp / 2 + Temp && Degree <= Temp * 2)
		{
			Index = 2;
		}
	}
	else
	{
		Index = 0;
	}

	if (Index >= 0)
	{
		CurrentInventoryIndex = Index;
		if (Inventory[Index])
		{
			switch (Inventory[Index]->GetToolType())
			{
			case EToolType::ToolType_Mop:
			{
				if (MopDescriptionTexture)
				{
					SetToolDescription(MopDescriptionTexture, 1.f);
					Inventory[Index]->SetOpacity(1.f);
					Inventory[Index]->IncreaseIconSizeAnim();
					
				}
			}
			break;
			case EToolType::ToolType_Flashlight:
			{
				if (FlashlightDescriptionTexture)
				{
					SetToolDescription(FlashlightDescriptionTexture, 1.f);
					Inventory[Index]->SetOpacity(1.f);
					Inventory[Index]->IncreaseIconSizeAnim();
				}
			}
			break;
			case EToolType::ToolType_Axe:
			{
				if (AxeDescriptionTexture)
				{
					SetToolDescription(AxeDescriptionTexture, 1.f);
					Inventory[Index]->SetOpacity(1.f);
					Inventory[Index]->IncreaseIconSizeAnim();
				}
			}
			break;
			case EToolType::ToolType_Sponge:
			{
				if (SpongeDescriptionTexture)
				{
					SetToolDescription(SpongeDescriptionTexture, 1.f);
					Inventory[Index]->SetOpacity(1.f);
					Inventory[Index]->IncreaseIconSizeAnim();
				}
			}
			break;
			case EToolType::ToolType_None:
			{
				if (HandDescriptionTexture)
				{
					SetToolDescription(HandDescriptionTexture, 1.f);
					Inventory[Index]->SetOpacity(1.f);
					Inventory[Index]->IncreaseIconSizeAnim();
				}
			}
			break;
			}

			for (int i = 0; i < Inventory.Num(); i++)
			{
				if (Index != i)
				{
					Inventory[i]->SetOpacity(0.f);
					Inventory[i]->SetToolIconOpacity(1.f);
					Inventory[Index]->DecreaseIconSizeAnim();
				}
			}
		}
	}
}



