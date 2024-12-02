// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioObject.h"
#include "Net\UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "Components\AudioComponent.h"
#include "OverlappedTrigger.h"


void ARadioObject::InterAction(APawn* Character)
{
	if (!bActivate)
	{
		SetObjectOwner(Character);
		PickUp(Character);
		bIsCleaning = false;
		OnRep_SetCleaningState();
	}
}

void ARadioObject::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
	if (bIsCleaning)
	{
		StaticMesh->CustomDepthStencilValue = 1;
	}
	else
	{
		StaticMesh->CustomDepthStencilValue = 2;
	}
}

void ARadioObject::UpdateToolTip(APlayerCharacter* Player)
{
}

void ARadioObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARadioObject, bActivate);
}

void ARadioObject::C2S_StopEvent_Implementation()
{
	
	S2A_StopEvent();
}
void ARadioObject::S2A_StopEvent_Implementation()
{
	if (HasAuthority())
	{
		if (OverlappedTrigger)
			OverlappedTrigger->Destroy();
	}
	bActivate = false;
	GetWorldTimerManager().ClearTimer(TimerHandle);
	if (EmissiveValue == 1.0f)
	{
		SetEmissive();
	}
	if (ObjectSound && ObjectSoundAudio)
	{
		ObjectSoundAudio->Stop();
	}
}

void ARadioObject::C2S_StartEvent_Implementation()
{
	bActivate = true;
	S2A_StartEvent();
}

void ARadioObject::S2A_StartEvent_Implementation()
{
	bActivate = true;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARadioObject::SetEmissive, 0.3f, true);
	if (ObjectSound && ObjectSoundAudio)
	{
		ObjectSoundAudio->Play();
	}
}

void ARadioObject::SetEmissive()
{
	if (EmissiveValue == 1.f)
	{
		EmissiveValue = 0.0f;
	}
	else
	{
		EmissiveValue = 1.0f;
	}
	StaticMesh->SetScalarParameterValueOnMaterials(FName("emissive"), EmissiveValue);
}
