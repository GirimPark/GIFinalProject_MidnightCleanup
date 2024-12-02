// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioObjectTrigger.h"
#include "RadioObject.h"
#include "SpawnPoint.h"

void ARadioObjectTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivate)
	{
		if(HasAuthority())
		{
			

			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			if (Player)
			{
				ARadioObject* Radio = Cast<ARadioObject>(TargetObject);
				if (Radio)
				{
					if(TriggerSpawnPoint&&TriggerClass)
					{
						FActorSpawnParameters SpawnParam;
						SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						FVector Location = TriggerSpawnPoint->GetActorLocation();
						FRotator Rotation = TriggerSpawnPoint->GetActorRotation();

						Radio->OverlappedTrigger = GetWorld()->SpawnActor<AOverlappedTrigger>(TriggerClass, Location, Rotation, SpawnParam);
					}

					Player->UpdateEnableTriggerCount();

					Radio->bActivate = true;
					Radio->C2S_StartEvent();
					C2S_SetActivate(false);
				}
			}
		}
	}
}
