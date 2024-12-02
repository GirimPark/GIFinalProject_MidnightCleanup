// Fill out your copyright notice in the Description page of Project Settings.


#include "OpacityDecal.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Seunggi\InGameGM.h"
#include "Seunggi\InGameGS.h"
#include "Kismet\GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "ToolTipData.h"
#include "Seunggi/InteractionPickUpObject.h"
#include "Sponge.h"

AOpacityDecal::AOpacityDecal()
{
	CurrentOpacity = 1.0f;
	bWasInteractive = false;
	Reduction = 0.01f;
}

void AOpacityDecal::BeginPlay()
{
	Super::BeginPlay();
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}

	if (MI)
	{
		MI->SetScalarParameterValue(FName("Opacity"), CurrentOpacity);
	}

	S2A_SetBoxExtent(Decal->DecalSize);
}

void AOpacityDecal::Destroyed()
{
	Super::Destroyed();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->EraseOpacityDecalNum++;
		GS->OnRep_UpdateCount();
	}
}

void AOpacityDecal::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		if (Player->OwnedActor && Player->OwnedActor->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Tool)
		{
			ASponge* Sponge = Cast<ASponge>(Player->OwnedActor);
			if (Sponge)
			{
				FToolTipData* ToolTip = nullptr;
				if ((Player->GetActorLocation() - GetActorLocation()).Length() <= 150)
				{
					ToolTip = GS->GetToolTipByID(FName(TEXT("SpongeDecal")));

					if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
					{
						Player->UpdateToolTip(ToolTip);
						Player->CurrentToolTipData = *ToolTip;
					}
				}
				else
				{
					ToolTip = GS->GetToolTipByID(FName(TEXT("DefaultToolTip")));
					Player->UpdateToolTip(ToolTip);
					Player->CurrentToolTipData = *ToolTip;
					UE_LOG(LogTemp, Warning, TEXT("ToolTip Data Change"));
				}
			}
		}
	}
}

void AOpacityDecal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOpacityDecal, CurrentOpacity);
	DOREPLIFETIME(AOpacityDecal, bWasInteractive);
	DOREPLIFETIME(AOpacityDecal, Reduction);
}

void AOpacityDecal::S2A_SetOpacity_Implementation(float Opacity)
{
	if (MI)
	{
		CurrentOpacity = Opacity;
		MI->SetScalarParameterValue(FName("Opacity"), CurrentOpacity);
	}
}

void AOpacityDecal::DrawOutline(bool Draw)
{
	
}

void AOpacityDecal::HintDraw(bool Draw)
{
	if (Draw)
	{
		MI->SetScalarParameterValue(TEXT("luminous"), 0.5);
	}
	else
	{
		MI->SetScalarParameterValue(TEXT("luminous"), 0);
	}
}

