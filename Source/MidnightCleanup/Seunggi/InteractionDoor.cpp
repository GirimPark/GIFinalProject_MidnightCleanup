// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionDoor.h"
#include "InGameGS.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerCharacter.h"
#include "Sound\SoundCue.h"
#include "Components/AudioComponent.h"
#include "Components\BoxComponent.h"

AInteractionDoor::AInteractionDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsOpened = false;
	ObjectType = EObjectType::ObjectType_Static;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Box"));
	StaticMesh->SetupAttachment(RootComponent);
	//StaticMesh->SetRelativeLocation(FVector(-110, 0, 0));
}

void AInteractionDoor::InterAction(APawn* Player)
{
	SetDoorSpeed(7.0f);
	DoorMove();
	S2A_SoundPlay();
}

void AInteractionDoor::DrawOutline(bool Draw)
{
}

void AInteractionDoor::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("Door")));

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;

	}
	}
}

void AInteractionDoor::BeginPlay()
{
	Super::BeginPlay();
	if (EventSound)
	{
		EventSoundAudio = NewObject<UAudioComponent>(this);
		EventSoundAudio->SetupAttachment(RootComponent);
		EventSoundAudio->SetSound(EventSound);  

		EventSoundAudio->RegisterComponent();
		EventSoundAudio->Stop();
	}
	
}

void AInteractionDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector ResultPosition;
	if (bIsOpened)
	{
		ResultPosition = FMath::VInterpTo(StaticMesh->GetRelativeLocation(), OpenedPosition, GetWorld()->GetDeltaSeconds(), DoorSpeed);
	}
	else
	{
		ResultPosition = FMath::VInterpTo(StaticMesh->GetRelativeLocation(), ClosedPosition, GetWorld()->GetDeltaSeconds(), DoorSpeed);
		
	}
	StaticMesh->SetRelativeLocation(ResultPosition);
}

void AInteractionDoor::DoorMove_Implementation()
{
	
	FVector ResultPosition;
	if (bIsOpened)
	{
		bIsOpened = false;
	}
	else
	{
		bIsOpened = true;
	}
}

void AInteractionDoor::SetDoorSpeed_Implementation(float Speed)
{
	DoorSpeed = Speed;
}

void AInteractionDoor::S2A_EventSoundPlay_Implementation()
{
	if(EventSound && EventSoundAudio)
	{
		EventSoundAudio->Play();
	}
}

