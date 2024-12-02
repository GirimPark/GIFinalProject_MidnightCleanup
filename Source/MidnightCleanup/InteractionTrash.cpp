// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionTrash.h"
#include "Components/BoxComponent.h"
#include "InteractionBox.h"
#include "Seunggi\InGameGM.h"
#include "Seunggi\InGameGS.h"
#include "Kismet\GameplayStatics.h"

AInteractionTrash::AInteractionTrash()
{
	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Trash;
}

void AInteractionTrash::BeginPlay()
{
	Super::BeginPlay();
	//Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractionTrash::OnBoxBeginOverlap);
	//Box->OnComponentEndOverlap.AddDynamic(this, &AInteractionTrash::OnBoxEndOverlap);
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}
	
}

void AInteractionTrash::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

void AInteractionTrash::Destroyed()
{
	Super::Destroyed();
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->TrashNum--;
		GS->OnRep_UpdateCount();
	}
}

void AInteractionTrash::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInteractionBox* InteractionBox = Cast<AInteractionBox>(OtherActor);
	if (InteractionBox)
	{
		if (HasAuthority())
		{
			SetReplicates(false);
		}
		BoxOwner = InteractionBox;
	}
	else
	{
		if (HasAuthority())
		{
			SetReplicates(true);
		}
		BoxOwner = nullptr;
	}
}

void AInteractionTrash::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInteractionBox* InteractionBox = Cast<AInteractionBox>(OtherActor);
	if (InteractionBox)
	{
		BoxOwner = InteractionBox;
	}
	else
	{
		BoxOwner = nullptr;
	}
}

