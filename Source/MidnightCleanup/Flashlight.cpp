// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"

AFlashlight::AFlashlight()
{
	SetName(TEXT("Flashlight"));
	SetToolType(EToolType::ToolType_Flashlight);
	MaxIntensity = 0.f;
	bIsActivate = false;
}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMesh)
	{
		MI = StaticMesh->CreateDynamicMaterialInstance(0);
	}
}

void AFlashlight::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority())
	{
		return;
	}

	if (!bIsActivate)
	{
		return;
	}

	if (ObjectOwner)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		if (!Player)
		{
			return;
		}

		if (this == Player->Inventory[Player->CurrentInventoryIndex])
		{
			if (Player->TPSFlashlight->GetAttachSocketName() == FName(TEXT("TPSFlashlight_OnHand")))
			{
				return;
			}
			Player->UpdateTPSFlashlightSocket(FName(TEXT("TPSFlashlight_OnHand")));
		}
		else
		{
			if (Player->TPSFlashlight->GetAttachSocketName() == FName(TEXT("TPSFlashlight_NotHand")))
			{
				return;
			}
			Player->UpdateTPSFlashlightSocket(FName(TEXT("TPSFlashlight_NotHand")));
		}
	}
}

void AFlashlight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlashlight, MaxIntensity);
}

void AFlashlight::S2A_SetFlashlightActivate_Implementation(bool State)
{
	bIsActivate = State;
	//UE_LOG(LogTemp, Warning, TEXT("IS ACTIVATE : %d"), bIsActivate);

	if (State)
	{
		OnFlashlight();
	}
	else
	{
		OffFlashlight();
	}
}

void AFlashlight::OffFlashlight()
{
	MI->SetScalarParameterValue(FName(TEXT("EmissiveMult")), 0.f);
	StaticMesh->SetMaterial(0, MI);

}

void AFlashlight::OnFlashlight()
{
	MI->SetScalarParameterValue(FName(TEXT("EmissiveMult")), 15.f);
	StaticMesh->SetMaterial(0, MI);

}


