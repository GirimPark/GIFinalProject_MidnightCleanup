// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComputer.h"
#include "PlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "Seunggi/InGameGS.h"
#include "Kismet/GameplayStatics.h"

AInteractionComputer::AInteractionComputer()
{
	CameraMovementPath = CreateDefaultSubobject<USplineComponent>(TEXT("CameraMovementPath"));
	if (CameraMovementPath)
	{
		CameraMovementPath->SetupAttachment(RootComponent);
	}
}

void AInteractionComputer::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player && !bIsActivate)
	{
		FVector Location = CameraMovementPath->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		Player->SetCameraLocation(Location);
		Player->MoveCameraToComputer(CameraMovementPath);
		S2A_SetComputerActivateState(true);
	}
}

void AInteractionComputer::DrawOutline(bool Draw)
{
	if (!bIsActivate)
	{
		StaticMesh->SetRenderCustomDepth(Draw);
	}
}

void AInteractionComputer::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("Computer")));

		if (!bIsActivate && ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
}

void AInteractionComputer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionComputer, CameraMovementPath);
	DOREPLIFETIME(AInteractionComputer, bIsActivate);
}

void AInteractionComputer::S2A_SetComputerActivateState_Implementation(bool State)
{
	bIsActivate = State;
}

