// Fill out your copyright notice in the Description page of Project Settings.


#include "StoveObject.h"
#include "Net\UnrealNetwork.h"
#include "SpawnPoint.h"
#include "InGameGM.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../TextureDecal.h"
#include "NiagaraFunctionLibrary.h"
#include "PotObject.h"
#include "Components/BoxComponent.h"
#include "OverlappedTrigger.h"

void AStoveObject::InterAction(APawn* Character)
{
	if (bActivate)
	{
		if(OverlappedTrigger)
			OverlappedTrigger->Destroy();
		bActivate = false;
		C2S_StopEvent();
		Pot->bActivate = false;
		Pot->C2S_StopEvent();
		Pot->S2A_SoundStop();
		S2A_SoundPlay();
	}
}

void AStoveObject::DrawOutline(bool Draw)
{
	if (bActivate)
	{
		StaticMesh->SetRenderCustomDepth(Draw);
	}
	else
	{
		StaticMesh->SetRenderCustomDepth(false);
	}
}

void AStoveObject::UpdateToolTip(APlayerCharacter* Player)
{
}

void AStoveObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStoveObject, bActivate);
}

void AStoveObject::C2S_StopEvent_Implementation()
{
	S2A_StopEvent();
	bActivate = false;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AStoveObject::S2A_StopEvent_Implementation()
{
	bActivate = false;
	DisableEffect();
}

void AStoveObject::C2S_StartEvent_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Event start"));
	bActivate = true;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AStoveObject::CreateDecal, StayTime, false);
	S2A_StartEvent();
	Pot->C2S_StartEvent();
	Pot->S2A_SoundPlay();
}

void AStoveObject::S2A_StartEvent_Implementation()
{
	bActivate = true;
	EnableEffect();
}

void AStoveObject::CreateDecal()
{
	if(DecalClass&&DecalSpawnPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT(" S2A_CreateDecal_Implementation Create Decal"));
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector Location = DecalSpawnPoint->GetActorLocation();
		FRotator Rotation = DecalSpawnPoint->GetActorRotation();

		ATextureDecal* GeneratedDecal = GetWorld()->SpawnActor<ATextureDecal>(DecalClass, Location, Rotation, SpawnParam);

		if (GeneratedDecal)
		{
			AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (!GM->SpawnDecal.IsEmpty())
			{
				ATextureDecal* PrevDecal = GM->SpawnDecal[GM->SpawnDecal.Num() - 1];
				int32 PrevSortOrder = PrevDecal->GetSortOrder();
				GeneratedDecal->S2A_SetSortOrder(PrevSortOrder + 1);
			}
			else
			{
				GeneratedDecal->S2A_SetSortOrder(1);
			}

			if (Pot)
			{
				ThrowVector.Normalize();

				Pot->StaticMesh->SetSimulatePhysics(true);
				Pot->StaticMesh
					->AddImpulse(ImpulsePower * ThrowVector);
				Pot->S2A_SoundStop();
			}
			GM->SpawnDecal.Add(GeneratedDecal);
			C2S_StopEvent();
		}
	}
}
