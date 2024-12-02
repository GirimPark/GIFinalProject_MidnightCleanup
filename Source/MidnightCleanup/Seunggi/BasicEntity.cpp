// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEntity.h"
#include "Components\BoxComponent.h"
#include "Net\UnrealNetwork.h"
#include "InGameGM.h"
#include "Kismet\GameplayStatics.h"
#include "Camera\CameraShakeBase.h"
#include "../PC.h"
#include "Sound\SoundCue.h"
#include "Components\PostProcessComponent.h"
#include "Components\AudioComponent.h"

// Sets default values

ABasicEntity::ABasicEntity()
{
	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComp->SetupAttachment(RootComponent);

	PostProcessComp->bEnabled = false;
	PostProcessComp->SetIsReplicated(false);

	bActivate = false;
}

void ABasicEntity::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABasicEntity::ProcessOverlap);
}

void ABasicEntity::OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if (Cast<APlayerCharacter>(OtherActor))
		{
			S2C_TriggerEvent(Cast<APlayerCharacter>(OtherActor), true);
		}
	}
}

void ABasicEntity::ProcessOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapFucntion(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABasicEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasicEntity, bActivate);
}

void ABasicEntity::C2S_SetActivate_Implementation(bool Activate)
{
	if (false == Activate)
	{
		GetWorld()->GetAuthGameMode<AInGameGM>()->UpdatePowerLevel(this->GetClass());
	}
	bActivate = Activate;
	
	S2A_SetActivate(Activate);
	//StoveOn();
}

void ABasicEntity::S2A_SetActivate_Implementation(bool Activate)
{
	bActivate = Activate;
	
#if WITH_EDITOR
	Box->SetHiddenInGame(!Activate);
#endif
	StoveOn();
}


void ABasicEntity::S2C_TriggerEvent_Implementation(APlayerCharacter* Player, bool Enabled)
{
	if(Player)
	{
		PostProcessComp->bEnabled = true;
		APC* PC = Cast<APC>(Player->GetController());
	
		//if(PC&&Player->IsLocallyControlled())
		//{
		//	FPostProcessSettings PPS = PostProcessComp->Settings;
		//	PC->ApplyPostProcessEffect(PPS, Enabled);
			FTimerHandle Timer;
			GetWorldTimerManager().SetTimer(Timer, [this, Player]() {
				SetPPActivate(false);
				}, PostProcessTime, false);
			
			TriggerEvent(Player);
		//}
	}
}


void ABasicEntity::TriggerEnd_Implementation(APlayerCharacter* Player, bool Enabled)
{
	if (Player)
	{
		PostProcessComp->bEnabled = Enabled;
		APC* PC = Cast<APC>(Player->GetController());
		//if (PC)
		//{
		//	FPostProcessSettings PPS = PostProcessComp->Settings;
		//	PC->ApplyPostProcessEffect(PPS, Enabled);
		//
		//}
	}
}


void ABasicEntity::StoveOn()
{
}

void ABasicEntity::TriggerEvent(APlayerCharacter* Player)
{
	APC* PlayerController = Player->GetController<APC>();
	if(Player->IsLocallyControlled())
	{
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(CameraShakeClass);
			if (TriggerSound)
			{
				TriggerSoundAudio = UGameplayStatics::SpawnSound2D(PlayerController, TriggerSound);
			}
		}
	}
	
}

void ABasicEntity::TriggerEventEnd(APlayerCharacter* Player)
{
	APC* PlayerController = Player->GetController<APC>();
	if (Player->IsLocallyControlled())
	{
		if (PlayerController)
		{
			PlayerController->ClientStopCameraShake(CameraShakeClass);
			if (TriggerSoundAudio->IsPlaying())
			{
				TriggerSoundAudio->Stop();
				//UGameplayStatics::PlaySound2D(PlayerController, TriggerSound,0.0f);
			}
		}
	}
}

void ABasicEntity::SetSound_Implementation(APlayerCharacter* Player, bool Activate)
{
	APC* PlayerController = Player->GetController<APC>();
	if (Player->IsLocallyControlled())
	{
		if (Activate)
		{
			if (PlayerController)
			{
				PlayerController->ClientStartCameraShake(CameraShakeClass);
				if (TriggerSound)
				{
					TriggerSoundAudio = UGameplayStatics::SpawnSound2D(PlayerController, TriggerSound);
				}
			}

		}
		else
		{

			if (PlayerController)
			{
				PlayerController->ClientStopCameraShake(CameraShakeClass);
				if(TriggerSoundAudio)
				{
					if (TriggerSoundAudio->IsPlaying())
					{
						TriggerSoundAudio->Stop();
						//UGameplayStatics::PlaySound2D(PlayerController, TriggerSound,0.0f);
					}
				}
			}

		}
	}

}



void ABasicEntity::SetPPActivate_Implementation(bool Activate)
{
	PostProcessComp->bEnabled = Activate;
}

