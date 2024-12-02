// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "InteractionPickUpObject.h"
#include "../PlayerCharacter.h"
#include "InGameGM.h"
#include "../InteractionBox.h"
#include "../InteractionBucket.h"
#include "InGameGS.h"
#include "Kismet/GameplayStatics.h"
#include "../InGamePS.h"



// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentObjectNum = 0;
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::InterAction(APawn* Character)
{
	if (SpawnObject)
	{
		AInGamePS* PS = Cast<AInGamePS>(Character->GetPlayerState());
		if (PS)
		{
			PS->SpawnBoxAndBucketCount++;
			PS->OnRep_UpdateReward();
		}

		AInGameGM* GM = GetWorld()->GetAuthGameMode<AInGameGM>();
		if (!GM)
		{
			return;
		}
		if (CurrentObjectNum < LimitNum)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector Location = GetActorLocation() + SpawnPoint;
			FTransform Transform = FTransform(Location);
			AInteractionPickUpObject* Object = GetWorld()->SpawnActor<AInteractionPickUpObject>(SpawnObject,Transform, SpawnParam);
			if (Object && Player)
			{
				Object->InterAction(Player);
				Player->C2S_AddActorsToIgnore(Object);
				Player->C2S_SetHeldState(true);
				Player->C2S_SetOwnedActor(Object);
			}
			GM->CountObject();
		}
			
	}
}

void ASpawner::DrawOutline(bool Draw)
{
}

void ASpawner::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("PickUp")));

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}

}

