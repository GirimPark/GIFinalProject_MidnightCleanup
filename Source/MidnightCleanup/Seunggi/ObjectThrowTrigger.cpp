// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectThrowTrigger.h"
#include "InteractionPickUpObject.h"
#include "Components/BoxComponent.h"
#include "Props.h"
#include "Components\PostProcessComponent.h"

void AObjectThrowTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivate)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			SetPPActivate(true);
			
			//UE_LOG(LogTemp, Warning, TEXT("Character overlap"));
			//Super::OverlapFucntion(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
			ABasicEntity::TriggerEvent(Player);
			if(HasAuthority())
			{
				C2S_SetActivate(false);
				FTimerHandle Timer;
				GetWorldTimerManager().SetTimer(Timer, this, &AObjectThrowTrigger::TriggerEvent, DelayTime, false);
				SetOwner(Player);
				Player->UpdateEnableTriggerCount();
			}
			FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer, [this]() {SetPPActivate(false); }, PostProcessTime, false);
		}
	}
}

void AObjectThrowTrigger::TriggerEvent()
{
	if (TargetObject)
	{
		AInteractionPickUpObject* Object = Cast<AInteractionPickUpObject>(TargetObject);
		if (Object && !Object->ObjectOwner)
		{
			AProps* Prop = Cast<AProps>(TargetObject);
			if (Prop)
			{
				Prop->bIsCleaning = false;
			}
			ThrowVector.Normalize();

			TargetObject->StaticMesh->SetSimulatePhysics(true);
			TargetObject->StaticMesh->AddImpulse(ImpulsePower * ThrowVector);
		}
	}
}
