// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWoodenPlank.h"
#include "InGameGS.h"
#include "InGameGM.h"
#include "Kismet/GameplayStatics.h"
#include "Components\BoxComponent.h"

AInteractionWoodenPlank::AInteractionWoodenPlank()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//Box->SetCollisionProfileName(TEXT("Interactable"));
	Box->SetupAttachment(RootComponent);

	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMesh->SetupAttachment(RootComponent);

	PickUpObjectType = EPickUpObjectType::PickUpObjectType_WoodenPlank;
}

void AInteractionWoodenPlank::BeginPlay()
{
	Super::BeginPlay();
	bIsCleaning = false;

	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}
}
