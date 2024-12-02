// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidgetBase.h"
#include "RadialUIBase.h"
#include "RadialSectionBase.h"
#include "BasicTool.h"
#include "PlayerCharacter.h"
#include "PC.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "FurnitureProgressBase.h"
#include "ToolTipWidgetBase.h"
#include "AimWidgetBase.h"
#include "LimitTimeBase.h"
#include "Seunggi/InGameGS.h"

void UInGameWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	RadialUIBox = Cast<URadialUIBase>(GetWidgetFromName(TEXT("RadialUI")));
	AimBox = Cast<UAimWidgetBase>(GetWidgetFromName(TEXT("Aim")));
	FurnitureProgressBox = Cast<UFurnitureProgressBase>(GetWidgetFromName(TEXT("FurnitureProgress")));
	ToolTipBox = Cast<UToolTipWidgetBase>(GetWidgetFromName(TEXT("ToolTip")));
	LimitTimeBox = Cast<ULimitTimeBase>(GetWidgetFromName(TEXT("InGame_LimitTime")));
}

void UInGameWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	if (RadialUIBox->ArrowMaterial)
	{
		float Degree = CalculateArrowAngle();
		RadialUIBox->ArrowMaterial->SetScalarParameterValue(FName("Angle"), Degree);
		RadialUIBox->UpdateInventory(Degree);
	}

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (!PC)
		{
			return;
		}

		if (GS->bPrimaryModifyPlayTime)
		{
			if (GS->CurrentPlayTime >= 0)
			{
				UpdatePlayTime(GS->CurrentPlayTime);
			}
		}
		else
		{
			if (GS->CurrentPlayTime >= GS->OriginPlayTime)
			{
				UpdatePlayTime(GS->OriginPlayTime);
			}
			else
			{
				UpdatePlayTime(GS->CurrentPlayTime);
			}
		}
	}
}

void UInGameWidgetBase::UpdateInventoryByPlayer(APlayerCharacter* Player)
{
	if (Player)
	{
		for (int i = 0; i < Player->Inventory.Num(); i++)
		{
			if (Player->Inventory[i])
			{
				ABasicTool* Tool = Cast<ABasicTool>(Player->Inventory[i]);
				if (Tool && RadialUIBox)
				{
					switch (Tool->GetToolType())
					{
					case EToolType::ToolType_Mop:
					{
						URadialSectionBase* Section = RadialUIBox->Inventory[i];
						if (Section)
						{
							Section->SetToolType(EToolType::ToolType_Mop);
							Section->SetOpacity(1.f);
						}
					}
					break;
					case EToolType::ToolType_Flashlight:
					{
						URadialSectionBase* Section = RadialUIBox->Inventory[i];
						if (Section)
						{
							Section->SetToolType(EToolType::ToolType_Flashlight);
							Section->SetOpacity(1.f);
						}	
					}
					break;
					case EToolType::ToolType_Axe:
					{
						URadialSectionBase* Section = RadialUIBox->Inventory[i];
						if (Section)
						{
							Section->SetToolType(EToolType::ToolType_Axe);
							Section->SetOpacity(1.f);
						}						
					}
					break;
					case EToolType::ToolType_Sponge:
					{
						URadialSectionBase* Section = RadialUIBox->Inventory[i];
						if (Section)
						{
							Section->SetToolType(EToolType::ToolType_Sponge);
							Section->SetOpacity(1.f);
						}						
					}
					break;
					}
				}
			}
			else
			{
				if (RadialUIBox)
				{
					URadialSectionBase* Section = RadialUIBox->Inventory[i];
					if (Section)
					{
						Section->SetToolType(EToolType::ToolType_None);
						Section->SetOpacity(1.f);
					}
				}
			}
		}

		ABasicTool* CurrentTool = Cast<ABasicTool>(Player->Inventory[Player->CurrentInventoryIndex]);
		if (CurrentTool)
		{
			if (RadialUIBox)
			{
				RadialUIBox->SetOpacity(1.0f);
				RadialUIBox->CurrentInventoryIndex = Player->CurrentInventoryIndex;
				RadialUIBox->SetMousePos(Player->CurrentInventoryIndex);

				URadialSectionBase* Section = RadialUIBox->Inventory[Player->CurrentInventoryIndex];
				if (Section)
				{
					Section->SetToolType(CurrentTool->GetToolType());
					Section->SetOpacity(1.0f);
				}			
			}
		}
		else
		{
			if (RadialUIBox)
			{
				RadialUIBox->SetOpacity(1.0f);
				RadialUIBox->CurrentInventoryIndex = Player->CurrentInventoryIndex;
				RadialUIBox->SetMousePos(Player->CurrentInventoryIndex);

				URadialSectionBase* Section = RadialUIBox->Inventory[Player->CurrentInventoryIndex];
				if (Section)
				{
					Section->SetToolType(EToolType::ToolType_None);
					Section->SetOpacity(1.0f);
				}
			}
		}
	}
}

float UInGameWidgetBase::CalculateArrowAngle()
{
	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	FVector2D Center;
	FVector2D MousePosition;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		Center = FVector2D(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);
		
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	}

	FVector2D VectorToMouse = MousePosition - Center;
	FVector2D Up(0.f, -1.f);

	VectorToMouse.Normalize();

	float Dot = FVector2D::DotProduct(VectorToMouse, Up);
	float Radian = FMath::Acos(Dot);

	float Degree = FMath::RadiansToDegrees(Radian);

	if (MousePosition.X < Center.X)
	{
		Degree = Degree * -1.f;
	}

	return Degree;
}

int32 UInGameWidgetBase::GetCurrentInventoryIndex()
{
	return RadialUIBox->CurrentInventoryIndex;
}

void UInGameWidgetBase::UpdateAimIcon(EIconType InType)
{
	if (AimBox)
	{
		AimBox->SetIconType(InType);
	}
}

void UInGameWidgetBase::UpdatePlayTime(int32 InSeconds)
{
	if (LimitTimeBox)
	{
		LimitTimeBox->UpdatePlayTime(InSeconds);
	}
}




