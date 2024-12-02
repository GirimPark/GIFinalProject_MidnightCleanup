// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicInteractionObject.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Perception\AIPerceptionStimuliSourceComponent.h"
#include "Sound\SoundCue.h"
#include "Components\AudioComponent.h"

// Sets default values
ABasicInteractionObject::ABasicInteractionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SetReplicates(true);
	
	bReplicates = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(TEXT("Interactable"));
	RootComponent = StaticMesh;
	
	StaticMesh->SetGenerateOverlapEvents(true);
	
	//StaticMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABasicInteractionObject::BeginPlay()
{
	Super::BeginPlay();
	if (ObjectSound)
	{
		ObjectSoundAudio = NewObject<UAudioComponent>(this);
		ObjectSoundAudio->SetupAttachment(RootComponent);
		ObjectSoundAudio->SetSound(ObjectSound);  // DropSound는 사운드 큐로 설정
		ObjectSoundAudio->SetIsReplicated(true);
		ObjectSoundAudio->RegisterComponent();
		ObjectSoundAudio->Stop();
	}
}

// Called every frame
void ABasicInteractionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicInteractionObject::S2A_SoundPlay_Implementation()
{
	if (ObjectSound && ObjectSoundAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound Play"));
		ObjectSoundAudio->Play();
	}
}

void ABasicInteractionObject::S2A_SoundStop_Implementation()
{
	ObjectSoundAudio->Stop();
}

