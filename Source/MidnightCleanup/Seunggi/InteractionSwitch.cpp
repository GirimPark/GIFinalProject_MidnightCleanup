// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSwitch.h"
#include "InteractionLightActor.h"
#include "Components\PointLightComponent.h"
#include "InGameGS.h"
#include "../PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AInteractionSwitch::AInteractionSwitch()
{
	Switch=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	Switch->SetupAttachment(StaticMesh);
}

void AInteractionSwitch::InterAction(APawn* Character)
{
	if (Lights.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Light is not connected"));
		return;
	}
	S2A_SoundPlay();

	uint8 bIsLightOn = false;
	for (AInteractionLightActor* Light : Lights)
	{
		if (Light)
		{
			if (Light->LightComponent->Intensity != 0)
			{
				bIsLightOn = true;

				UE_LOG(LogTemp, Warning, TEXT("Turn Off"));
				SetLightingState(false);
				SetIntensity(0, Light);
			}
			else
			{
				SetLightingState(true);
				SetIntensity(Light->LightIntensity, Light);
			}
		}
	}
	
	if (bIsLightOn)
	{
		SetOwner(Character);

		APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
		if (Player)
		{
			Player->UpdateEnableTriggerCount();
		}
	}
}

void AInteractionSwitch::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator ResultRotation = Switch->GetRelativeRotation();
	if ((bIsLighting && ResultRotation == TurnOnRotation) || (!bIsLighting&&ResultRotation == TurnOffRotation))
	{
		return;
	}

	if (bIsLighting)
	{
		ResultRotation = FMath::RInterpTo(ResultRotation, TurnOnRotation, DeltaTime, 15);
	}
	else
	{
		ResultRotation = FMath::RInterpTo(ResultRotation, TurnOffRotation, DeltaTime, 15);
	}
	Switch->SetRelativeRotation(ResultRotation);
}

void AInteractionSwitch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionSwitch, bIsLighting);
}

void AInteractionSwitch::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = nullptr;
		if (bIsLighting)
		{
			ToolTip = GS->GetToolTipByID(FName(TEXT("SwitchOff")));
		}
		else
		{
			ToolTip = GS->GetToolTipByID(FName(TEXT("SwitchOn")));
		}

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
}

void AInteractionSwitch::BeginPlay()
{
	Super::BeginPlay();

}


void AInteractionSwitch::SetIntensity_Implementation(float Value, AInteractionLightActor* LightActor)
{
	Cast<ULightComponent>(LightActor->LightComponent)->SetIntensity(Value);
	if (Value == 0)
	{
		LightActor->StaticMesh->SetScalarParameterValueOnMaterials(FName("emissive"), 0.0f);
		bIsLighting = false;
	}
	else
	{
		LightActor->StaticMesh->SetScalarParameterValueOnMaterials(FName("emissive"), 0.7f);
		bIsLighting = true;
	}
}

void AInteractionSwitch::SetLightingState_Implementation(bool State)
{
	bIsLighting = State;
}

void AInteractionSwitch::Initialize()
{
	if(Lights.Num()>0)
	{
		for (AInteractionLightActor* Light : Lights)
		{
			if (Light)
			{
				Light->Switch = this;
				if (Light->LightComponent)
				{
					if (Light->LightComponent->Intensity > 0)
					{
						Light->LightIntensity = Light->LightComponent->Intensity;
						SetIntensity(0, Light);
					}
				}
			}
		}
		SetLightingState(false);
	}
}
