// Fill out your copyright notice in the Description page of Project Settings.


#include "LightOffTrigger.h"
#include "Components\BoxComponent.h"
#include "InteractionSwitch.h"
#include "Net/UnrealNetwork.h"

void ALightOffTrigger::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentEndOverlap.AddDynamic(this, &ALightOffTrigger::ProcessEndOverlap);
}

void ALightOffTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivate)
	{
		if(HasAuthority())
		{
			//UE_LOG(LogTemp, Warning, TEXT("OtherActor Overlap"));
			if (Cast<APlayerCharacter>(OtherActor))
			{
				if (AlreadyInPlayer&&GetWorldTimerManager().IsTimerActive(TimerHandle))
				{
					return;
				}
				AlreadyInPlayer = true;
				GetWorldTimerManager().SetTimer(TimerHandle, [this, OtherActor]()
					{
						TriggerStay(Cast<APlayerCharacter>(OtherActor));
					}, StayTime, false);

			}
		}
		
	}
	
}

void ALightOffTrigger::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
			AlreadyInPlayer = false;
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void ALightOffTrigger::TriggerStay(APawn* Player)
{
	AInteractionSwitch* Switch = Cast<AInteractionSwitch>(TargetObject);
	if (Switch && Switch->bIsLighting)
	{
		TArray<AActor*> OverlappingActors;
		Box->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
		for (auto& Actor : OverlappingActors)
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
			S2C_TriggerEvent(PlayerCharacter,true);
		}
		C2S_SetActivate(false);
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, [Switch, Player]()
			{
				Switch->InterAction(Player);
			}, StayTime, false);

	}
}

void ALightOffTrigger::OnRep_AlreadyPeople()
{
}

void ALightOffTrigger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALightOffTrigger, AlreadyInPlayer);
}
