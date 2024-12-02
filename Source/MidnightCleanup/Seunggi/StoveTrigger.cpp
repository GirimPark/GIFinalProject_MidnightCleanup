// Fill out your copyright notice in the Description page of Project Settings.


#include "StoveTrigger.h"
#include "StoveObject.h"
#include "Components\BoxComponent.h"
#include "SpawnPoint.h"

void AStoveTrigger::BeginPlay()
{
	Super::BeginPlay();
	//Box->OnComponentBeginOverlap.Clear();
}

void AStoveTrigger::StoveOn()
{
	AStoveObject* Stove = Cast<AStoveObject>(TargetObject);
	if(HasAuthority())
	{
		

		if (Stove && !Stove->bActivate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stove On"));
			Stove->C2S_StartEvent();
			if (TriggerSpawnPoint&&TriggerClass)
			{
				FActorSpawnParameters SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				FVector Location = TriggerSpawnPoint->GetActorLocation();
				FRotator Rotation = TriggerSpawnPoint->GetActorRotation();
				Stove->OverlappedTrigger = GetWorld()->SpawnActor<AOverlappedTrigger>(TriggerClass, Location, Rotation, SpawnParam);
			}
			C2S_SetActivate(false);
		}
	}
}

void AStoveTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
