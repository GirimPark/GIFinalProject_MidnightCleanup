// Fill out your copyright notice in the Description page of Project Settings.


#include "Props.h"
#include "ObjectOrganize.h"
#include "InGameGS.h"
#include "Kismet/GameplayStatics.h"
#include "../ToolTipData.h"
#include "../PlayerCharacter.h"
#include "Components\BoxComponent.h"

AProps::AProps()
{
	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Props;
}

void AProps::BeginPlay()
{
	Super::BeginPlay();
	if (bIsCleaning)
	{
		if (!OriginalLocation)
			return;
		if(HasAuthority())
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FTransform Transform = FTransform(GetActorRotation(), GetActorLocation());
			AObjectOrganize* SpawnedOriginalLocation = GetWorld()->SpawnActor<AObjectOrganize>(OriginalLocation, Transform, SpawnParam);

			SpawnedOriginalLocation->TargetObject = this;
			SpawnedOriginalLocation->Initialize();
			SpawnedOriginalLocation->S2A_AttachObject(this);
		}
	}
}

void AProps::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("PickUp")));
		if (ToolTip && bIsCleaning)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}

}

void AProps::DrawOutline(bool Draw)
{
	Super::DrawOutline(Draw);
	if (bIsCleaning)
	{
		StaticMesh->CustomDepthStencilValue = 1;
	}
	else
	{
		StaticMesh->CustomDepthStencilValue = 2;
	}
}

