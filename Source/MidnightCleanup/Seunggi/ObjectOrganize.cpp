// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectOrganize.h"
#include "../PlayerCharacter.h"
#include "Components\BoxComponent.h"
#include "InteractionPickUpObject.h"
#include "InGameGS.h"
#include "Kismet/GameplayStatics.h"
#include "Net\UnrealNetwork.h"
#include "../InGamePS.h"
#include "Props.h"

void AObjectOrganize::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AObjectOrganize::ProcessBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AObjectOrganize::ProcessEndOverlap);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Initialize();


}

void AObjectOrganize::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TargetObject)
		return;
	if (TargetObject->bIsCleaning)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetVisibility(false);
		return;
	}
	TArray<AActor*> Array;
	Box->GetOverlappingActors(Array);
	for (AActor* Actor : Array)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if (Player && Player->IsLocallyControlled())
		{
			if (StaticMesh->IsVisible())
			{
				if (Player->OwnedActor == nullptr || Player->OwnedActor != TargetObject)
				{
					StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					StaticMesh->SetVisibility(false);
				}
			}
			else
			{
				if (Player->OwnedActor == TargetObject)
				{
					StaticMesh->SetVisibility(true);
					StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}
			}
		}
	}
}

AObjectOrganize::AObjectOrganize()
{
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	RootComponent = Box;
	StaticMesh->SetupAttachment(RootComponent);
}

void AObjectOrganize::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		if (Player->OwnedActor == nullptr)
		{
			return;
		}
		else
		{
			if (TargetObject == Player->OwnedActor)
			{
				Player->DropObject();
				TargetObject->bIsCleaning = true;
				TargetObject->OnRep_SetCleaningState();
				S2A_AttachObject(TargetObject);

				AProps* Props = Cast<AProps>(TargetObject);
				if (Props)
				{
					AInGamePS* PS = Cast<AInGamePS>(Player->GetPlayerState());
					if (PS)
					{
						PS->OrganizePropsCount++;
						PS->OnRep_UpdateReward();
					}
				}
			}
		}
	}
}

void AObjectOrganize::DrawOutline(bool Draw)
{
	
}

void AObjectOrganize::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		if (StaticMesh && StaticMesh->IsVisible())
		{
			FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("FurnitureOrganize")));
			if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
			{
				Player->UpdateToolTip(ToolTip);
				Player->CurrentToolTipData = *ToolTip;
			}
		}
	}

}

void AObjectOrganize::DrawHologram(bool Draw)
{
	StaticMesh->SetVisibility(Draw);
}

void AObjectOrganize::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player&&Player->IsLocallyControlled()&&TargetObject == Player->OwnedActor)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DrawHologram(true);
	}
}

void AObjectOrganize::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && Player->IsLocallyControlled() && StaticMesh->IsVisible())
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DrawHologram(false);
	}
}

void AObjectOrganize::S2A_AttachObject_Implementation(AInteractionPickUpObject* Object)
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);

	Object->DetachFromActor(DetachmentRules);
	Object->AttachToComponent(StaticMesh, AttachmentRules);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AObjectOrganize::Initialize_Implementation()
{
	if (TargetObject)
	{
		StaticMesh->SetStaticMesh(TargetObject->StaticMesh->GetStaticMesh());
		StaticMesh->SetWorldScale3D(TargetObject->StaticMesh->GetComponentScale());
		FVector Min, Max;
		StaticMesh->GetLocalBounds(Min, Max);

		// Calculate the size of the bounding box
		FVector BoundingBoxSize = Max - Min;
		//StaticMesh->AddRelativeLocation(FVector(0, 0, -BoundingBoxSize.Z / 2));
		if (MaterialInstance)
		{
			StaticMesh->SetMaterial(0, MaterialInstance);
			StaticMesh->SetMaterial(1, MaterialInstance);
		}
		StaticMesh->SetVisibility(false);
	}
}

void AObjectOrganize::CheckOrganize()
{
	
}

void AObjectOrganize::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AObjectOrganize, TargetObject);
}

