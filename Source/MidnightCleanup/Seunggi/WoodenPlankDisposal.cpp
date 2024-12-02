// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodenPlankDisposal.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "InteractionWoodenPlank.h"
#include "InGameGM.h"
#include "InGameGS.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWoodenPlankDisposal::AWoodenPlankDisposal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	 
	RootComponent = Box;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWoodenPlankDisposal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWoodenPlankDisposal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Box->GetOverlappingActors(Array);
	for (AActor* Actor : Array)
	{
		if (Actor->IsA(AInteractionWoodenPlank::StaticClass()))
		{
			AInteractionWoodenPlank* WoodenPlank = Cast<AInteractionWoodenPlank>(Actor);
			if (WoodenPlank)
			{
				IsFullyOverlapped(WoodenPlank);
			}
		}
	}
}

bool AWoodenPlankDisposal::IsFullyOverlapped(AInteractionWoodenPlank* WoodenPlank)
{
	if (!WoodenPlank)
	{
		return false;
	}
	FVector Origin, BoxExtent;
	Origin = WoodenPlank->GetActorLocation();
	BoxExtent = WoodenPlank->Box->GetScaledBoxExtent();

	FVector Location = GetActorLocation();
	FVector Extent = Box->GetScaledBoxExtent();

	FVector Corners[8];
	Corners[0] = FVector(BoxExtent.X, BoxExtent.Y, BoxExtent.Z);
	Corners[1] = FVector(BoxExtent.X, BoxExtent.Y, -BoxExtent.Z);
	Corners[2] = FVector(BoxExtent.X, -BoxExtent.Y, BoxExtent.Z);
	Corners[3] = FVector(BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z);
	Corners[4] = FVector(-BoxExtent.X, BoxExtent.Y, BoxExtent.Z);
	Corners[5] = FVector(-BoxExtent.X, BoxExtent.Y, -BoxExtent.Z);
	Corners[6] = FVector(-BoxExtent.X, -BoxExtent.Y, BoxExtent.Z);
	Corners[7] = FVector(-BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z);

	FTransform WoodenPlankTransform = WoodenPlank->GetTransform();
	FTransform CleanTransform = Box->GetComponentTransform();
	AInGameGS* GS = GetWorld()->GetGameState<AInGameGS>();
	for (int32 i = 0; i < 8; i++)
	{
		
		FVector Corner = WoodenPlankTransform.TransformPosition(Corners[i]);
		FVector LocalVertex = CleanTransform.InverseTransformPositionNoScale(Corner);

		
		if (LocalVertex.X < -Extent.X ||
			LocalVertex.X >Extent.X ||
			LocalVertex.Y < -Extent.Y ||
			LocalVertex.Y >Extent.Y ||
			LocalVertex.Z < -Extent.Z ||
			LocalVertex.Z >Extent.Z)
		{
			
			WoodenPlank->bIsCleaning = false;	
			AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->CountObject();
			}
			return false; // If any corner is outside, the object is not fully inside
		}
	}

	WoodenPlank->bIsCleaning = true;
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}
	return true;
}

