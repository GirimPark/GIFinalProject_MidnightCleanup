// Fill out your copyright notice in the Description page of Project Settings.


#include "Sponge.h"
#include "Components/BoxComponent.h"
#include "OpacityDecal.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "InGamePS.h"
#include "Kismet/GameplayStatics.h"
#include "PC.h"
#include "Seunggi/InGameGS.h"

ASponge::ASponge()
{
	SetName(TEXT("Sponge"));
	SetToolType(EToolType::ToolType_Sponge);
	MaxDurability = 200;
	CleanDurability = 101;
	DirtyDurability = 0;
	Reduction = 1;
}

void ASponge::BeginPlay()
{
	Super::BeginPlay();
	FVector BoxSize = Box->GetScaledBoxExtent()+FVector(2,2,2);
	Box->SetBoxExtent(BoxSize);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ASponge::BoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ASponge::BoxEndOverlap);

	Durability = MaxDurability;
}

void ASponge::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Durability <= 0)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		if (Player && Player->Inventory[Player->CurrentInventoryIndex] == this)
		{
			APC* PC = Cast<APC>(Player->GetController());
			if (!PC)
			{
				return;
			}

			if (!PC->bPrimaryOldSponge)
			{
				AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
				if (!GS)
				{
					return;
				}

				FCaptionData* Data = GS->GetCaptionByID(FName(TEXT("PrimaryNotify_OldSponge")));
				if (!Data)
				{
					return;
				}

				Player->UpdateCaption(Data, 1.0f);

				PC->bPrimaryOldSponge = true;
			}
		}
	}
}

void ASponge::C2S_SetMaxDurability_Implementation()
{
	Durability = MaxDurability;

	S2A_SetSpongeDurability(Durability);
}

void ASponge::C2S_DecreaseDurability_Implementation(APawn* Character)
{
	if (OverlappedDecal)
	{
		if (OverlappedDecal->CurrentOpacity  > 0.1)
		{
			if (Durability > 0)
			{
				float Temp = OverlappedDecal->CurrentOpacity - OverlappedDecal->Reduction;
				OverlappedDecal->S2A_SetOpacity(Temp);

				if (OverlappedDecal->CurrentOpacity > 0.1)
				{
					Durability -= Reduction;
				}
				else
				{
					OverlappedDecal->S2A_SetOpacity(0.f);
					DestroyOverlappedDecal(Character);
				}
			}
							
			S2A_SetSpongeDurability(Durability);		
			S2A_SetSpongeMaterial(Durability);

			if (ObjectOwner)
			{
				APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
				ASponge* TPSSponge = Cast<ASponge>(Player->OwnedActor);
				if (TPSSponge)
				{
					TPSSponge->S2A_SetSpongeDurability(Durability);
					TPSSponge->S2A_SetSpongeMaterial(Durability);
				}
			}
		}
		else
		{
			DestroyOverlappedDecal(Character);
		}
	}
}

void ASponge::S2A_SetSpongeDurability_Implementation(int32 InDurability)
{
	Durability = InDurability;
}

void ASponge::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOpacityDecal* Decal = Cast<AOpacityDecal>(OtherActor);
	if (Decal)
	{
		OverlappedDecal = Decal;

		if (!ObjectOwner)
		{
			return;
		}

		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		if (Player)
		{
			if (HasAuthority())
			{
				Player->bIsEraseStart = true;
				Player->OnRep_SetEraseStartState();
				S2A_SetOverlappedDecal(Decal);
			}
			else
			{
				Player->SetSpongeOverlappedDecal(true, Decal);
			}
		}
	}
}

void ASponge::BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AOpacityDecal* Decal = Cast<AOpacityDecal>(OtherActor);
	if (Decal && Decal == OverlappedDecal)
	{
		OverlappedDecal = nullptr;

		if (!ObjectOwner)
		{
			return;
		}

		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		if (Player)
		{
			if (HasAuthority())
			{
				Player->bIsEraseStart = false;
				Player->OnRep_SetEraseStartState();
				S2A_SetOverlappedDecal(nullptr);
			}
			else
			{
				Player->SetSpongeOverlappedDecal(false, nullptr);
			}
		}
	}
}

void ASponge::S2A_SetSpongeActivate_Implementation(bool State)
{
	bIsActivate = State;
}

void ASponge::S2A_DestoryOverlappedDecal_Implementation()
{
	if (OverlappedDecal)
	{
		OverlappedDecal->Destroy();
		OverlappedDecal = nullptr;
		//UE_LOG(LogTemp, Warning, TEXT("Destroy OverlappedDecal"));
	}
}

void ASponge::SetOverlappedDecal(bool EraseState, AOpacityDecal* Decal)
{
	if (!ObjectOwner)
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
	if (Player)
	{
		Player->bIsEraseStart = EraseState;
		Player->OnRep_SetEraseStartState();
		
		S2A_SetOverlappedDecal(Decal);
	}
}

void ASponge::S2A_SetOverlappedDecal_Implementation(AOpacityDecal* Decal)
{
	if (!Decal)
	{
		return;
	}

	if (!ObjectOwner)
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
	if (!Player)
	{
		return;
	}

	if (Player->FPSOwnedActor == this)
	{
		OverlappedDecal = Decal;

		if (bIsActivate && !Decal->bWasInteractive)
		{
			float Temp = Decal->Reduction * 100;
			for (int i = 0; i < OpacityRatio / Temp; i++)
			{
				if (Decal->CurrentOpacity > 0.1)
				{
					if (Durability > 0)
					{
						float Opacity = Decal->CurrentOpacity - Decal->Reduction;
						Decal->S2A_SetOpacity(Opacity);

						if (Decal->CurrentOpacity > 0.1)
						{
							Durability -= Reduction;
						}
						else
						{
							Decal->S2A_SetOpacity(0.f);
							S2A_DestoryOverlappedDecal();
						}
					}

					S2A_SetSpongeDurability(Durability);
					S2A_SetSpongeMaterial(Durability);

					ASponge* TPSSponge = Cast<ASponge>(Player->OwnedActor);
					if (TPSSponge)
					{
						TPSSponge->S2A_SetSpongeDurability(Durability);
						TPSSponge->S2A_SetSpongeMaterial(Durability);
					}
		
				}
				else
				{
					S2A_DestoryOverlappedDecal();
				}

			}

			Decal->bWasInteractive = true;
		}
	}
}

void ASponge::S2A_CreateSpongeMaterial_Implementation()
{
	MI = StaticMesh->CreateDynamicMaterialInstance(0);
}

void ASponge::S2A_SetSpongeMaterial_Implementation(int32 InDurability)
{
	SetSpongeMaterial(InDurability);
}

void ASponge::DestroyOverlappedDecal(APawn* Character)
{
	S2A_DestoryOverlappedDecal();

	AInGamePS* PS = Cast<AInGamePS>(Character->GetPlayerState());
	if (PS)
	{
		PS->EraseDecalCount++;
		PS->OnRep_UpdateReward();
	}
}

void ASponge::S2A_SetFPSSponge_Implementation(AActor* InPlayer, int32 InDurability, int32 InMaxDurability, int32 InCleanDurability, int32 InDirtyDurability, float InOpacityRatio)
{
	ObjectOwner = InPlayer;
	Durability = InDurability;
	MaxDurability = InMaxDurability;
	CleanDurability = InCleanDurability;
	DirtyDurability = InDirtyDurability;
	OpacityRatio = InOpacityRatio;

	SetSpongeMaterial(InDurability);
}

void ASponge::SetSpongeMaterial(int32 InDurability)
{
	if (InDurability >= CleanDurability)
	{
		static FString Path = TEXT("/Game/Materials/interactive_object/Dirty_object_material/sponge/MI_Sponge_v1.MI_Sponge_v1");
		MI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *Path));
	}
	else if (InDurability <= DirtyDurability)
	{
		static FString Path = TEXT("/Game/Materials/interactive_object/Dirty_object_material/sponge/MI_Sponge_v3.MI_Sponge_v3");
		MI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *Path));
	}
	else
	{
		static FString Path = TEXT("/Game/Materials/interactive_object/Dirty_object_material/sponge/MI_Sponge_v2.MI_Sponge_v2");
		MI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *Path));
	}

	if (MI)
	{
		UE_LOG(LogTemp, Warning, TEXT("MI is Exist"))
			StaticMesh->SetMaterial(0, MI);
	}
}

void ASponge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASponge, OpacityRatio);
	DOREPLIFETIME(ASponge, CleanDurability);
	DOREPLIFETIME(ASponge, DirtyDurability);
	DOREPLIFETIME(ASponge, MI);
	DOREPLIFETIME(ASponge, OverlappedDecal);
}
