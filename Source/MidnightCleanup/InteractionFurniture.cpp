// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFurniture.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Axe.h"
#include "Net\UnrealNetwork.h"
#include "Seunggi\InteractionWoodenPlank.h"
#include "Seunggi/InGameGM.h"
#include "PC.h"
#include "Seunggi/InGameGS.h"
#include "GeometryCollection\GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "Seunggi\FurnitureFieldSystemActor.h"
#include "InGamePS.h"
#include "Components\AudioComponent.h"

AInteractionFurniture::AInteractionFurniture()
{
	ObjectType = EObjectType::ObjectType_Furniture;
	
	Geometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	RootComponent = Geometry;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetCollisionProfileName(TEXT("Interactable"));
	Geometry->SetSimulatePhysics(false);
	if(StaticMesh)
	{
		StaticMesh->DestroyComponent();
	}
}

void AInteractionFurniture::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
	//StaticMesh->SetVisibility(false);
	if (bBreak)
	{
		AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->CountObject();
		}
	}
}

void AInteractionFurniture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionFurniture::InterAction(APawn* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Furniture : %d"),Geometry->GetCollisionObjectType());
}

void AInteractionFurniture::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("FurnitureBreak")));

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
}

void AInteractionFurniture::Destroyed()
{
	Super::Destroyed();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->BreakFurnitureNum--;
		GS->OnRep_UpdateCount();
	}
}

void AInteractionFurniture::DrawOutline(bool state)
{
	Geometry->SetRenderCustomDepth(state);
}

void AInteractionFurniture::CreateTrash(uint8 TrashNum)
{
	for (int i = 0; i <TrashNum;i++)
	{
		if(Trash.Num()>0)
		{
			int index = FMath::RandRange(0, Trash.Num() - 1);
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FTransform Transform = FTransform(GetActorRotation(), GetActorLocation()+FVector(0,0,50), FVector(1.f, 1.f, 1.f));
			GetWorld()->SpawnActor<AInteractionWoodenPlank>(Trash[index], Transform, SpawnParam);
		}
	}
}

void AInteractionFurniture::Axe_Hit(APawn* Character)
{
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Axe Player"));
		AAxe* Axe = Cast<AAxe>(Player->OwnedActor);
		if (Axe)
		{
			UE_LOG(LogTemp, Warning, TEXT("AXE"));
			Axe->S2A_EnableEffect();
			CurrentHp -= 1;
			S2A_SoundPlay();
			if (CurrentHp == 0)
			{
				
				AInGamePS* PS = Cast<AInGamePS>(Player->GetPlayerState());
				if (PS)
				{
					PS->BreakFurnitureCount++;
					PS->OnRep_UpdateReward();
				}
				S2A_BreakFurniture(5000000);
				CreateTrash(SpawnTrashNum);
				
			}
		}
	}
}

void AInteractionFurniture::S2A_BreakFurniture_Implementation(float Magnitude)
{
	Geometry->SetSimulatePhysics(true);
	Box->SetCollisionObjectType(ECC_Destructible);
	Geometry->SetCollisionObjectType(ECC_Destructible);
	TArray<USceneComponent*, FDefaultAllocator> Components;
	Geometry->GetChildrenComponents(true, Components);
	for (auto& Component : Components)
	{
		Component->DestroyComponent();
	}
	if (ObjectSoundAudio)
	{
		ObjectSoundAudio->Play();
	}
	if (FieldSystem == nullptr)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform Transform = FTransform(GetActorRotation(), GetActorLocation() + FVector(0, 0, 100), GetActorScale());

		FieldSystem = GetWorld()->SpawnActor<AFurnitureFieldSystemActor>(AFurnitureFieldSystemActor::StaticClass(), Transform, SpawnParam);
		if (FieldSystem)
		{
			FName name = FieldSystem->GetFName();
		}
	}
	OpacityValue = 10;
	FieldSystem->ApplyField(Magnitude);
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AInteractionFurniture::SelfDestroy, 0.2f, true);
}

void AInteractionFurniture::SelfDestroy()
{
	OpacityValue -= 1;
	Geometry->SetScalarParameterValueOnMaterials("Opacity", OpacityValue);
	if(OpacityValue<1.f)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
}

void AInteractionFurniture::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionFurniture, MaxHp);
	DOREPLIFETIME(AInteractionFurniture, CurrentHp);
}


