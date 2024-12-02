// Fill out your copyright notice in the Description page of Project Settings.


#include "HandPrintTrigger.h"
#include "SpawnPoint.h"
#include "../TextureDecal.h"
#include "Sound\SoundCue.h"
#include "Components\AudioComponent.h"
#include "Components\PostProcessComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InGameGS.h"

void AHandPrintTrigger::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivate)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if(Player)
		{
			PostProcessComp->bEnabled = true;
			TriggerEvent(Player);
			FTimerHandle Timer1;
			GetWorldTimerManager().SetTimer(Timer1, [this]() {SetPPActivate(false); }, PostProcessTime, false);
			
			if ((OtherActor->GetActorLocation() - HandPrintPoint[0]->GetActorLocation()).SquaredLength() > (GetActorLocation() - HandPrintPoint[0]->GetActorLocation()).SquaredLength())
			{
				if (ObjectSound)
				{
					ObjectSoundAudio = NewObject<UAudioComponent>(this);
					ObjectSoundAudio->SetupAttachment(RootComponent);
					ObjectSoundAudio->SetSound(ObjectSound);

					ObjectSoundAudio->RegisterComponent();
					ObjectSoundAudio->Stop();
				}

				Index = 0;

				if (HasAuthority())
				{
					Player->UpdateEnableTriggerCount();
					FTimerHandle Timer;
					GetWorldTimerManager().SetTimer(Timer, this, &AHandPrintTrigger::C2S_SpawnHandPrint, DelayTime, false);
					C2S_SetActivate(false);
				}
				
			}
		}

	}

}

void AHandPrintTrigger::C2S_SpawnHandPrint_Implementation()
{
	if (Index < HandPrintPoint.Num())
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (!GS)
		{
			return;
		}

		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform Transform = FTransform(HandPrintPoint[Index]->GetActorRotation(), HandPrintPoint[Index]->GetActorLocation());
		if (Index % 2 == 0)
		{
			GetWorld()->SpawnActor<AOpacityDecal>(RightHand, Transform, SpawnParam);
			GS->CreateOpacityDecalCount++;
			GS->OnRep_UpdateCount();
		}
		else
		{

			GetWorld()->SpawnActor<AOpacityDecal>(LeftHand, Transform, SpawnParam);
			GS->CreateOpacityDecalCount++;
			GS->OnRep_UpdateCount();
		}
		S2A_SoundPlay(Transform.GetLocation());
		Index++;
		if (Index < HandPrintPoint.Num())
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHandPrintTrigger::C2S_SpawnHandPrint, HandPrintTime[Index - 1], false);
		}
	}
}

void AHandPrintTrigger::S2A_SoundPlay_Implementation(FVector Location)
{
	if(ObjectSoundAudio&&ObjectSound)
	{
		ObjectSoundAudio->SetWorldLocation(Location);
		ObjectSoundAudio->Play();
	}
}

