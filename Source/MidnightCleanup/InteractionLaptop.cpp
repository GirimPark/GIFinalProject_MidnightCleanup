// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionLaptop.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"

// Sets default values
AInteractionLaptop::AInteractionLaptop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("Interactable"));
	if (Box)
	{
		RootComponent = Box;
	}
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh)
	{
		StaticMesh->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AInteractionLaptop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionLaptop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionLaptop::Interaction(APlayerCharacter* Player)
{
	if (Player)
	{
		//Player->EnableKeypad();
	}
}

