// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBucket.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Mop.h"
#include "Sponge.h"
#include "Seunggi\InGameGM.h"
#include "ToolTipData.h"
#include "NiagaraSystem.h"
#include "Components\BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TextureDecal.h"
#include "Seunggi/InGameGS.h"
#include "InGamePS.h"



AInteractionBucket::AInteractionBucket()
{
	WaterPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterPlane"));
	WaterPlane->SetupAttachment(StaticMesh);

	SpoilDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("SpoilDetector"));
	SpoilDetector->SetupAttachment(RootComponent);

	

	PickUpObjectType = EPickUpObjectType::PickUpObjectType_Bucket;
	BucketState = EBucketState::BucketState_Clean;
	Durability = MaxDurability;
}

void AInteractionBucket::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionBucket, Durability);
}

inline void AInteractionBucket::BeginPlay()
{
	Super::BeginPlay();
	
	//bIsFilled = false;
	bIsFilled ? Durability = MaxDurability : Durability = 0;


	SpoilDetector->OnComponentBeginOverlap.AddDynamic(this, &AInteractionBucket::ProcessBeginOverlap);
}

void AInteractionBucket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInteractionBucket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FBox Box1Bounds = SpoilDetector->Bounds.GetBox();
	FBox Box2Bounds = WaterPlane->Bounds.GetBox();

	UE_LOG(LogTemp, Warning, TEXT("Box1 Location : %f %f %f"),  Box1Bounds.Min.X, Box1Bounds.Min.Y,  Box1Bounds.Min.Z);
	UE_LOG(LogTemp, Warning, TEXT("Box2 Location : %f %f %f"), Box2Bounds.Min.X, Box2Bounds.Min.Y,  Box2Bounds.Min.Z);
	UE_LOG(LogTemp, Warning, TEXT("Actor Location : %f %f %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	if(bIsFilled && (Box1Bounds.Intersect(Box2Bounds)))
	{
		
		if (HasAuthority())
		{
			S2A_SoundPlay();
			if (Durability <= NormalDurability)
			{
				bool Result;

				TArray<AActor*> ActorsToIgnore;

				FHitResult OutHit;
				TArray<TEnumAsByte<EObjectTypeQuery>> Objects1;

				Objects1.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				//Objects1.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
				FVector TraceStart = GetActorLocation();
				FVector TraceEnd = TraceStart - FVector(0, 0, 1000);

				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects1,true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue,FLinearColor::Yellow, 5.0f);

				if (Result)
				{
					CreateDecal(OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation());
				}
			}

			
		}
		bIsFilled = false;
		EnableSpoilEffect();
	}
}

void AInteractionBucket::Destroyed()
{
	Super::Destroyed();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->BucketNum--;
		GS->OnRep_UpdateCount();
	}
}

void AInteractionBucket::UpdateToolTip(APlayerCharacter* Player)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		FToolTipData* ToolTip = nullptr;
		if (Player->OwnedActor && Player->OwnedActor->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Tool)
		{
			ABasicTool* Tool = Cast<ABasicTool>(Player->OwnedActor);
			if (Tool)
			{
				if (Tool->GetToolType() == EToolType::ToolType_Mop)
				{
					if (Tool->Durability < Tool->MaxDurability)
					{
						ToolTip = GS->GetToolTipByID(FName(TEXT("CleaningTool")));
					}
				}
				else if (Tool->GetToolType() == EToolType::ToolType_Sponge)
				{
					ABasicTool* Sponge = Cast<ABasicTool>(Player->FPSOwnedActor);
					if (Sponge && (Sponge->Durability < Sponge->MaxDurability))
					{
						ToolTip = GS->GetToolTipByID(FName(TEXT("CleaningTool")));
					}
				}
			}
		}
		else
		{
			ToolTip = GS->GetToolTipByID(FName(TEXT("PickUp")));
		}

		if (ToolTip && Player->CurrentToolTipData.Priority >= ToolTip->Priority)
		{
			Player->UpdateToolTip(ToolTip);
			Player->CurrentToolTipData = *ToolTip;
		}
	}
}

void AInteractionBucket::PickUpObject(APawn* Character)
{
	Super::PickUpObject(Character);
	SpoilDetector->SetCollisionProfileName(TEXT("Interactable"));
}

void AInteractionBucket::DropProcess(APawn* Character)
{
	Super::DropProcess(Character);
	SpoilDetector->SetCollisionProfileName(TEXT("Box"));
}

void AInteractionBucket::Cleaning(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		ABasicTool* Tool = Cast<ABasicTool>(Player->OwnedActor);
		if (Tool && bIsFilled)
		{
			UNiagaraSystem* NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effect/FXS_Wash_Splash.FXS_Wash_Splash"));
			S2A_SetAndEnableEffect(NiagaraSystem);

			if (Tool->ToolType == EToolType::ToolType_Mop)
			{
				AMop* Mop = Cast<AMop>(Tool);
				AMop* FPSMop = Cast<AMop>(Player->FPSOwnedActor);
				if (Mop && FPSMop)
				{
					if (Durability > 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("Set Durability"));
						Durability -= (Mop->MaxDurability - Mop->Durability);
						Mop->Durability = Mop->MaxDurability;
						Mop->S2A_SetMopDurability(Mop->MaxDurability);
						Mop->S2A_SetMopMaterial(Mop->MaxDurability);

						FPSMop->S2A_SetMopDurability(Mop->MaxDurability);
						FPSMop->S2A_SetMopMaterial(Mop->MaxDurability);

						if (Durability < 0)
						{
							Durability = 0;
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Set Durability 0"));
						Mop->Durability = 0;
						Mop->S2A_SetMopDurability(0);
						Mop->S2A_SetMopMaterial(0);

						FPSMop->S2A_SetMopDurability(0);
						FPSMop->S2A_SetMopMaterial(0);
					}
				}
			}
			else if (Tool->ToolType == EToolType::ToolType_Sponge)
			{
				ASponge* Sponge = Cast<ASponge>(Tool);
				ASponge* FPSSponge = Cast<ASponge>(Player->FPSOwnedActor);

				if (FPSSponge && Sponge)
				{
					if (Durability > 0)
					{
						Durability -= ((FPSSponge->MaxDurability - FPSSponge->Durability)/SpongeDecrease);

						FPSSponge->Durability = FPSSponge->MaxDurability;
						FPSSponge->S2A_SetSpongeDurability(FPSSponge->MaxDurability);
						FPSSponge->S2A_SetSpongeMaterial(FPSSponge->MaxDurability);

						Sponge->S2A_SetSpongeDurability(FPSSponge->MaxDurability);
						Sponge->S2A_SetSpongeMaterial(FPSSponge->MaxDurability);

						if (Durability < 0)
						{
							Durability = 0;
						}
					}
					else
					{
						FPSSponge->Durability = 0;
						FPSSponge->S2A_SetSpongeDurability(0);
						FPSSponge->S2A_SetSpongeMaterial(0);

						Sponge->S2A_SetSpongeDurability(0);
						Sponge->S2A_SetSpongeMaterial(0);
					}

				}
			}
			OnRep_ChangeState();
		}
	}
}

void AInteractionBucket::CreateDecal(FVector CreatePoint, FRotator CreateRotator)
{
	UE_LOG(LogTemp, Warning, TEXT(" S2A_CreateDecal_Implementation Create Decal"));
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = CreatePoint;
	FRotator Rotation = CreateRotator;

	ATextureDecal* GeneratedDecal = GetWorld()->SpawnActor<ATextureDecal>(DecalClass, Location, Rotation, SpawnParam);

	if (GeneratedDecal)
	{
		AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!GM->SpawnDecal.IsEmpty())
		{
			ATextureDecal* PrevDecal = GM->SpawnDecal[GM->SpawnDecal.Num() - 1];
			int32 PrevSortOrder = PrevDecal->GetSortOrder();
			GeneratedDecal->S2A_SetSortOrder(PrevSortOrder + 1);
		}
		else
		{
			GeneratedDecal->S2A_SetSortOrder(1);
		}

		GM->SpawnDecal.Add(GeneratedDecal);

		APawn* Character = Cast<APawn>(LastOwner);
		if (Character)
		{
			AInGamePS* PS = Cast<AInGamePS>(Character->GetPlayerState());
			if (PS)
			{
				PS->CreateDecalCount++;
				PS->SpillBucketCount++;
				PS->OnRep_UpdateReward();
			}
		}
	}
}

void AInteractionBucket::OnRep_ChangeState()
{
	UE_LOG(LogTemp, Warning, TEXT("change Bucket State"));
	if (Durability > NormalDurability)
	{
		BucketState = EBucketState::BucketState_Clean;
		WaterPlane->SetMaterial(0, ClaenMaterial);
	}
	else if (Durability <= DirtyDurability)
	{
		BucketState = EBucketState::BucketState_Dirty;
		WaterPlane->SetMaterial(0, DirtyMaterial);
	}
	else
	{
		BucketState = EBucketState::BucketState_Normal;
		WaterPlane->SetMaterial(0, NormalMaterial);
	}
}

void AInteractionBucket::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (LastOwner == OtherActor)
		{
			return;
		}
		S2A_SetObjectOwner(Cast<APlayerCharacter>(OtherActor));
	}
}

void AInteractionBucket::S2A_SetAndEnableEffect_Implementation(UNiagaraSystem* System)
{
	if (System)
	{
		SetEffect(System);
		EnableEffect();
	}
}


