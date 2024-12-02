// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFaucet.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "../PlayerCharacter.h"
#include "../InteractionBucket.h"
#include "InGameGS.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

AInteractionFaucet::AInteractionFaucet()
{
	Hologram = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HologramMesh"));
	Hologram->SetupAttachment(RootComponent);
	//Box->SetSimulatePhysics(false);
}

void AInteractionFaucet::BeginPlay()
{
	Super::BeginPlay();
	Hologram->SetVisibility(false);
}

void AInteractionFaucet::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		if (Player->OwnedActor == nullptr)
		{
			return;
		}
		else
		{
			AInteractionBucket* Bucket = Cast<AInteractionBucket>(Player->OwnedActor);
			if (Bucket)
			{
				Player->DropObject();
				Bucket->bIsFilled = true;
				Bucket->Durability = Bucket->MaxDurability;
				S2A_FillBucket(Bucket);
				Bucket->OnRep_ChangeState();
				S2A_SoundPlay();
			}
		}
	}
}

void AInteractionFaucet::DrawOutline(bool Draw)
{
	Hologram->SetVisibility(Draw);
}

void AInteractionFaucet::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("FillBucket")));

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
	
}

void AInteractionFaucet::S2A_FillBucket_Implementation(class AInteractionBucket* Bucket)
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld,true);

	UNiagaraSystem* NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effect/FXS_Fill_Splash.FXS_Fill_Splash"));
	Bucket->S2A_SetAndEnableEffect(NiagaraSystem);
	Bucket->AttachToComponent(Hologram, AttachmentRules);
	Bucket->bIsFilled = true;
	//Bucket->Durability = Bucket->MaxDurability;
	Bucket->DetachFromActor(DetachmentRules);
}
