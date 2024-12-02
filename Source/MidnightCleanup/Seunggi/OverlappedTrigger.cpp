// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlappedTrigger.h"
#include "Components\PostProcessComponent.h"
#include "Components\BoxComponent.h"

void AOverlappedTrigger::BeginPlay()
{
	Super::BeginPlay();
	PostProcessComp->bEnabled = true;

	Box->OnComponentEndOverlap.AddDynamic(this, &AOverlappedTrigger::ProcessEndOverlap);
}

void AOverlappedTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		PlayerCharacter = Player;
		
			if (PlayerCharacter)
			{
				//if (HasAuthority())
				{
					SetSound(PlayerCharacter, true);
					//ABasicEntity::TriggerEvent(PlayerCharacter);
				}
			}
		
	}
}

void AOverlappedTrigger::Destroyed()
{
	TArray<AActor*> Character;
	Box->GetOverlappingActors(Character, APlayerCharacter::StaticClass());
	for (auto Player : Character)
	{
		APlayerCharacter* Playerc = Cast<APlayerCharacter>(Player);
		if (Playerc)
		{
			SetSound(Playerc, false);
		}
	}
	

}

void AOverlappedTrigger::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		//if (PlayerCharacter==Player)
		{
			//if (HasAuthority())
			{
				SetSound(Player, false);
			}
		}

	}
}
