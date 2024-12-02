// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorCloseTrigger.h"
#include "InteractionDoor.h"
#include "Components\BoxComponent.h"
#include "Net\UnrealNetwork.h"

void ADoorCloseTrigger::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentEndOverlap.AddDynamic(this, &ADoorCloseTrigger::ProcessEndOverlap);
}

void ADoorCloseTrigger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoorCloseTrigger, TimerHandle);
}

void ADoorCloseTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivate)
	{
		if (Cast<APlayerCharacter>(OtherActor))
		{
			if (GetWorldTimerManager().IsTimerActive(TimerHandle))
			{
				return;
			}
			if (HasAuthority())
			{
				GetWorldTimerManager().SetTimer(TimerHandle, [this, OtherActor]()
					{
						TriggerStay(Cast<APlayerCharacter>(OtherActor));
					}, DelayTime, false);
			}
			
		}
	}
}

void ADoorCloseTrigger::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (HasAuthority())
		{
			TArray<AActor*> OverlapActors;
			Box->GetOverlappingActors(OverlapActors, APlayerCharacter::StaticClass());
			if (OverlapActors.Num() > 0)
			{
				return;
			}
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void ADoorCloseTrigger::C2S_CloseDoor_Implementation(APlayerCharacter* Player)
{
	AInteractionDoor* Door = Cast<AInteractionDoor>(TargetObject);
	AInteractionDoor* Door2 = Cast<AInteractionDoor>(TargetObject2);

	if (Door && Door2)
	{
		if (Door->bIsOpened ^ Door2->bIsOpened)
		{
			Player->UpdateEnableTriggerCount();

			if (Door->bIsOpened)
			{
				Door->SetDoorSpeed(10.0f);
				Door->DoorMove();
				Door->S2A_EventSoundPlay();
			}
			else
			{
				Door2->SetDoorSpeed(10.0f);
				Door2->DoorMove();
				Door2->S2A_EventSoundPlay();
			}
		}
		
	}
}

void ADoorCloseTrigger::TriggerStay(APlayerCharacter* Player)
{
	if (Player)
	{
		//if (HasAuthority())
		{
			AInteractionDoor* Door = Cast<AInteractionDoor>(TargetObject);
			AInteractionDoor* Door2 = Cast<AInteractionDoor>(TargetObject2);

			if (Door && Door2)
			{
				if (Door->bIsOpened ^ Door2->bIsOpened)
				{

					FTimerHandle Timer;
					GetWorldTimerManager().SetTimer(Timer, [this, Player]()
						{
							C2S_CloseDoor(Player);
						}, DelayTime, false);
					C2S_SetActivate(false);
				}
				TArray<AActor*> OverlappingActors;
				Box->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
				for (auto& Actor : OverlappingActors)
				{
					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
					S2C_TriggerEvent(PlayerCharacter, true);
				}
			}
		}
		
	}
}
