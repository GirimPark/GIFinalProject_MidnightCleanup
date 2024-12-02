// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBox.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Seunggi/BasicGhost.h"
#include "InteractionTrash.h"
#include "Seunggi\InGameGM.h"
#include "Seunggi\InGameGS.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AInteractionBox::AInteractionBox()
{
	CloseBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseBox"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	//Box->SetCollisionProfileName(TEXT("Interactable"));
	Box->SetupAttachment(RootComponent);
	CloseBox->SetupAttachment(RootComponent);
	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Box;
	Trashes.SetNum(0);
	
	//Box->SetCollisionProfileName(TEXT("Interactable"));
}

void AInteractionBox::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractionBox::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AInteractionBox::OnBoxEndOverlap);

}

void AInteractionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TrashNum = Trashes.Num();
}

void AInteractionBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionBox, bIsOverlap);
	DOREPLIFETIME(AInteractionBox, TrashNum);
}

void AInteractionBox::Destroyed()
{
	Super::Destroyed();
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->BucketNum--;
		GS->OnRep_UpdateCount();
	}
}

void AInteractionBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInteractionBox::SetBoxSize(int32 InSize)
{
	//Trashes.SetNum(InSize);
}

void AInteractionBox::AddTrash(AInteractionTrash* InTrash)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Trash Num : %d"), Trashes.Num());
	Trashes.Emplace(InTrash);
}

void AInteractionBox::RemoveTrash(AInteractionTrash* InTrash)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Trash Num : %d"), Trashes.Num());
	Trashes.Remove(InTrash);
}

void AInteractionBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AInteractionBox::OnBoxBeginOverlap"));
	AInteractionTrash* Trash = Cast<AInteractionTrash>(OtherActor);
	if (Trash)
	{
		AddTrash(Trash);
	}
}

void AInteractionBox::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInteractionTrash* Trash = Cast<AInteractionTrash>(OtherActor);
	if (Trash)
	{
		RemoveTrash(Trash);
	}
}

void AInteractionBox::PickUpTrash(APawn* Character)
{
	for (AInteractionTrash* Trash : Trashes)
	{
		//Trash->PickUp(Character);
	}
}

void AInteractionBox::DropTrash(APawn* Character)
{
	for (AInteractionTrash* Trash : Trashes)
	{
		//Trash->Drop(Character);
	}
}

void AInteractionBox::PickUpObject(APawn* Character)
{
	Super::PickUpObject(Character);
	CloseBox->SetCollisionProfileName(TEXT("Interactable"));
}

void AInteractionBox::DropProcess(APawn* Character)
{
	Super::DropProcess(Character);
	CloseBox->SetCollisionProfileName(TEXT("Box"));
}

