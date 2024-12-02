// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMonitor.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Seunggi/InGameGS.h"
#include "ToolTipData.h"
#include "CleanProgressMonitor.h"
#include "CleanAreaMonitorBase.h"
#include "Components/WidgetInteractionComponent.h"
#include "PC.h"

// Sets default values
AInteractionMonitor::AInteractionMonitor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	if (Widget)
	{
		Widget->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AInteractionMonitor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractionMonitor::InterAction(APawn* Character)
{
}

void AInteractionMonitor::DrawOutline(bool Draw)
{
}

void AInteractionMonitor::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		ACleanProgressMonitor* Monitor = Cast<ACleanProgressMonitor>(this);
		if (!Monitor)
		{
			UCleanAreaMonitorBase* CleanAreaMonitorWidget = Cast<UCleanAreaMonitorBase>(Widget->GetWidget());
			if (CleanAreaMonitorWidget)
			{
				if (Player->Trace3DWidget->IsOverInteractableWidget())
				{
					FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("CleanAreaMonitor")));

					if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
					{
						Player->UpdateToolTip(ToolTip);
						Player->CurrentToolTipData = *ToolTip;
					}
				}
			}
		}
	}
	
}

// Called every frame
void AInteractionMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


