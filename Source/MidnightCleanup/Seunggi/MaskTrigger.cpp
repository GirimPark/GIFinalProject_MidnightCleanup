// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskTrigger.h"
#include "MaskObject.h"
#include "SpawnPoint.h"

void AMaskTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(bActivate)
		{
			if(Cast<APlayerCharacter>(OtherActor))
			{
				C2S_SpawnMask();
				C2S_SetActivate(false);
			}
		}
	}
}

void AMaskTrigger::C2S_SpawnMask_Implementation()
{
	if (MaskObject&&MaskSpawnPoint)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform Transform = FTransform(MaskSpawnPoint->GetActorRotation(),MaskSpawnPoint->GetActorLocation());
		AMaskObject* Object = GetWorld()->SpawnActor<AMaskObject>(MaskObject, Transform, SpawnParam);
		if (Object)
		{
			Object->S2A_EnableEffect();
		}

	}
}

