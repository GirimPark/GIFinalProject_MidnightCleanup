// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);

	bIsOverlap = false;
	bIsActivate = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

		Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveObject::ProcessBeginOverlap);
		Box->OnComponentEndOverlap.AddDynamic(this, &AInteractiveObject::ProcessEndOverlap);
	}
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveObject, bIsOverlap);
	DOREPLIFETIME(AInteractiveObject, bIsActivate);
	DOREPLIFETIME(AInteractiveObject, MaterialInterface);
}

void AInteractiveObject::OnRep_Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Activate"));
	ActivateSequence();
}

void AInteractiveObject::OnRep_Overlap()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Overlap"));
	OverlapSequence();
}

void AInteractiveObject::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("ProcessBeginOverlap"));

			bIsOverlap = true;
			
			//Pawn->AddInteractiveObject(this);
		}
	}
}

void AInteractiveObject::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("ProcessEndOverlap"));

			bIsOverlap = false;

			//Pawn->RemoveInteractiveObject();
		}
	}
}

void AInteractiveObject::C2S_SetOverlap_Implementation(bool State)
{
	UE_LOG(LogTemp, Warning, TEXT("C2S_SetOverlap_Implementation"));
	bIsOverlap = State;
}

void AInteractiveObject::S2C_SetOverlap_Implementation(bool State)
{
	UE_LOG(LogTemp, Warning, TEXT("S2C_SetOverlap_Implementation"));
	bIsOverlap = State;
}


