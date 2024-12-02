// Fill out your copyright notice in the Description page of Project Settings.


#include "PotObject.h"
#include "Net\UnrealNetwork.h"
#include "Kismet\KismetStringLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "../CaptionData.h"
#include "InGameGS.h"
#include "../PlayerCharacter.h"
#include "InGameGM.h"
#include "../TextureDecal.h"
#include "NiagaraFunctionLibrary.h"
#include "Components\BoxComponent.h"

void APotObject::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentHit.AddDynamic(this, &APotObject::HitProcess);
}

void APotObject::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (!Player)
	{
		return;
	}
	if (bActivate)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		int32 Temp = FMath::RandRange(1, 3);
		FString Key = UKismetStringLibrary::Concat_StrStr(TEXT("Interaction_ToPot_"), FString::FromInt(Temp));
		FCaptionData* Data = GS->GetCaptionByID(FName(Key));
		if (Data)
		{
			Player->UpdateCaption(Data, 1.0f);
		}
	}
	else
	{
		if (!Player->bIsHeld)
		{
			StaticMesh->SetSimulatePhysics(true);
			SetObjectOwner(Character);
			PickUp(Character);
			bIsCleaning = false;
			OnRep_SetCleaningState();
		}
	}
}



void APotObject::UpdateToolTip(APlayerCharacter* Player)
{
}

void APotObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APotObject, bActivate);
}

void APotObject::C2S_StopEvent_Implementation()
{
	S2A_StopEvent();
	bActivate = false;
}

void APotObject::S2A_StopEvent_Implementation()
{
	bActivate = false;
}

void APotObject::C2S_StartEvent_Implementation()
{
	bActivate = true;
	S2A_StartEvent();
}

void APotObject::S2A_StartEvent_Implementation()
{
	bActivate = true;
}

void APotObject::CreateDecal(FVector HitLocation, FRotator HitRotator)
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = HitLocation;
	FRotator Rotation = HitRotator;

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

		GM->SpawnDecal.Add(GeneratedDecal);
		
		AInGameGS* GS = Cast<AInGameGS>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CreateTextureDecalCount++;
			GS->OnRep_UpdateCount();
		}

		C2S_StopEvent();
	}
}

void APotObject::HitProcess(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!bActivate)
		return;
	if (HasAuthority())
	{
		if (OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
		{
			CreateDecal(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
}
