// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTool.h"
#include "Flashlight.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Mop.h"
#include "Sponge.h"
#include "Seunggi/InGameGS.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasicTool::ABasicTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Tool;	

	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	RootComponent = Box;
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABasicTool::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetSimulatePhysics(false);
}

// Called every frame
void ABasicTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString ABasicTool::GetName()
{
	return Name;
}

void ABasicTool::SetName(FString InName)
{
	Name = InName;
}

void ABasicTool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicTool, Durability);
	DOREPLIFETIME(ABasicTool, MaxDurability);
	DOREPLIFETIME(ABasicTool, bIsActivate);
	DOREPLIFETIME(ABasicTool, Reduction);
}

void ABasicTool::DropPreviousTool(APlayerCharacter* Player)
{
	S2A_DropPreviousTool(Player);
}

void ABasicTool::DetachPreviousSocket(APlayerCharacter* Player)
{
	S2A_DetachTool(Player);
}

EToolType ABasicTool::GetToolType()
{
	return ToolType;
}

void ABasicTool::SetToolType(EToolType InToolType)
{
	ToolType = InToolType;
}

void ABasicTool::VisibleObject(bool State)
{
	S2A_VisibleTool(State);
}

void ABasicTool::PickUpObject(APawn* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Pick up tool"));

	Box->SetCollisionProfileName(TEXT("Tool"));
	if (ToolType == EToolType::ToolType_Mop)
	{
		AMop* Mop = Cast<AMop>(this);
		if (Mop)
		{
			Mop->SetCollisionType(TEXT("Box"));
		}
	}
	else
	{
		StaticMesh->SetCollisionProfileName(TEXT("Box"));
	}

	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);
		
		switch (GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			Box->SetSimulatePhysics(false);
			Player->SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Mop"));	

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Mop"));
			}
		}
		break;
		case EToolType::ToolType_Axe:
		{
			Box->SetSimulatePhysics(false);
			Player->SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Axe"));

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Axe"));
			}
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			Box->SetSimulatePhysics(false);
			//Box->SetCollisionProfileName(TEXT("NoCollision"));
			Player->SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Sponge"));	

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Sponge"));
				
			}
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			Box->SetSimulatePhysics(false);
			Player->SetToolHeldState(this, true);
			AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Flashlight"));

			if (Player->FPSOwnedActor)
			{
				Player->FPSOwnedActor->AttachToComponent(Player->FPSMesh, AttachmentRules, FName("Flashlight"));
			}

			AFlashlight* Flashlight = Cast<AFlashlight>(this);
			if (Flashlight)
			{
				Player->SetFlashlightState(Flashlight, Flashlight->bIsActivate);
			}
		}
		break;
		}

		if (Player->IsLocallyControlled())
		{
			if (GetToolType() == EToolType::ToolType_Mop)
			{
				AMop* Mop = Cast<AMop>(this);
				if (Mop)
				{
					Mop->SkeletalMesh->SetVisibility(false);
				}
			}
			else
			{
				StaticMesh->SetVisibility(false);
			}
			
		}
		else
		{
			if (GetToolType() == EToolType::ToolType_Mop)
			{
				UE_LOG(LogTemp, Warning, TEXT("FPS Mesh visible"));
				AMop* Mop = Cast<AMop>(this);
				if (Mop)
				{
					Mop->SkeletalMesh->SetVisibility(true);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FPS Mesh visible"));
				StaticMesh->SetVisibility(true);
			}
		}
	}
}

void ABasicTool::DropProcess(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		FVector Forward = Player->GetActorForwardVector();
		
		FDetachmentTransformRules DetachmentRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			false
		);

		switch (GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			Player->SetToolHeldState(this, false);
		}
		break;
		case EToolType::ToolType_Axe:
		{
			Player->SetToolHeldState(this, false);
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			DropSponge(Player);
			//Box->SetCollisionProfileName(TEXT("Tool"));
			Player->SetToolHeldState(this, false);		
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			Player->SetToolHeldState(this, false);

			AFlashlight* Flashlight = Cast<AFlashlight>(this);
			if (Flashlight)
			{
				Player->SetFPSFlashlight(Flashlight, false);
				Player->SetTPSFlashlight(Flashlight, false);
			}
		}
		break;
		}

		DetachFromActor(DetachmentRules);
		
		Box->SetSimulatePhysics(true);

		if (Player->FPSOwnedActor)
		{
			Player->FPSOwnedActor->Destroy();
			Player->FPSOwnedActor = nullptr;
			
			AMop* Mop = Cast<AMop>(this);
			if (Mop)
			{
				Mop->SkeletalMesh->SetVisibility(true);
			}
			else
			{
				StaticMesh->SetVisibility(true);
			}
		}
		SetActorLocation(GetActorLocation() + (Forward * DetachDistance));
	}
}

void ABasicTool::PickUpTool(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if(Player)
	{
		
	}
}

void ABasicTool::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = GS->GetToolTipByID(FName(TEXT("PickUp")));
		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
}

void ABasicTool::S2A_VisibleTool_Implementation(bool State)
{
	if (StaticMesh)
	{
		StaticMesh->SetVisibility(State);
	}
}

void ABasicTool::S2A_DropPreviousTool_Implementation(APlayerCharacter* Player)
{
	if (Player)
	{
		DropProcess(Player);
		Box->SetSimulatePhysics(false);
		Box->SetCollisionProfileName(TEXT("NoCollision"));
		if (ToolType == EToolType::ToolType_Mop)
		{
			AMop* Mop = Cast<AMop>(this);
			if (Mop)
			{
				Mop->SetCollisionType(TEXT("NoCollision"));
			}
		}
		else
		{
			StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
		}

		Sphere->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void ABasicTool::S2A_DetachTool_Implementation(APlayerCharacter* Player)
{
	if (Player)
	{
		FDetachmentTransformRules DetachmentRules(
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepWorld,
				EDetachmentRule::KeepWorld,
				false
			);

		DetachFromActor(DetachmentRules);
		
		//PickUpTool(Player);
	}
}

void ABasicTool::S2A_AttachPreviousTool_Implementation(APlayerCharacter* Player)
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);

	if (!Player->IsAttachActor(FName(TEXT("inventory_1"))))
	{
		AttachToComponent(Player->GetMesh(), AttachmentRules, FName(TEXT("inventory_1")));
	}
	else if (!Player->IsAttachActor(FName(TEXT("inventory_2"))))
	{
		AttachToComponent(Player->GetMesh(), AttachmentRules, FName(TEXT("inventory_2")));
	}
	else if (!Player->IsAttachActor(FName(TEXT("inventory_3"))))
	{
		AttachToComponent(Player->GetMesh(), AttachmentRules, FName(TEXT("inventory_3")));
	}
}

void ABasicTool::PickUpFPSSponge(APlayerCharacter* Player)
{
	if (Player && Player->FPSOwnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUpSponge"));
		ABasicTool* Tool = Cast<ABasicTool>(Player->FPSOwnedActor);
		if(Tool)
		{
			Player->FPSOwnedActor->S2A_SetOverlapEvents(true);
			Tool->Box->SetSimulatePhysics(true);
			Player->PhysicsHandle->GrabComponentAtLocationWithRotation(Tool->Box, NAME_None, Player->FPSOwnedActor->GetActorLocation(), Player->FPSOwnedActor->GetActorRotation());
		}
	}
}

void ABasicTool::DropSponge(APlayerCharacter* Player)
{
	if (Player)
	{
		Player->PhysicsHandle->ReleaseComponent();		
		Player->OwnedActor = nullptr;
	}
}


