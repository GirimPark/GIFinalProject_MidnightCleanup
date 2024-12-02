// Fill out your copyright notice in the Description page of Project Settings.


#include "Mop.h"
#include "TextureDecal.h"
#include "Kismet/GameplayStatics.h"
#include "Seunggi/InGameGM.h"
#include "Net/UnrealNetwork.h"
#include "TextureDecal.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "Components\AudioComponent.h"
#include "Sound\SoundCue.h"
#include "PC.h"
#include "Seunggi/InGameGS.h"

AMop::AMop()
{
	SetName(TEXT("Mop"));
	SetToolType(EToolType::ToolType_Mop);
	EraseCount = 1;
	MaxDurability = 10;
	Durability = MaxDurability;
	Reduction = 1;
	DecalSize = FVector{1.f, 128.f, 128.f};
	CleanDurability = 6.f;
	DirtyDurability = 0.f;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}

void AMop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMop, EraseCount);
	DOREPLIFETIME(AMop, DecalSize);
	DOREPLIFETIME(AMop, CleanDurability);
	DOREPLIFETIME(AMop, DirtyDurability);
}

void AMop::BeginPlay()
{
	Super::BeginPlay();
	if (SwingSound)
	{
		SwingSoundAudio = NewObject<UAudioComponent>(this);
		SwingSoundAudio->SetupAttachment(RootComponent);
		SwingSoundAudio->SetSound(SwingSound);  // DropSound는 사운드 큐로 설정

		SwingSoundAudio->RegisterComponent();
		SwingSoundAudio->Stop();
	}

	HeadMI = SkeletalMesh->CreateDynamicMaterialInstance(1);
	StickMI = SkeletalMesh->CreateDynamicMaterialInstance(0);

	Durability = MaxDurability;
	//UE_LOG(LogTemp, Warning, TEXT("BEGINPLAY : MOP"));
}

void AMop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

			if (!PC->bPrimaryOldMop)
			{
				AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
				if (!GS)
				{
					return;
				}

				FCaptionData* Data = GS->GetCaptionByID(FName(TEXT("PrimaryNotify_OldMop")));
				if (!Data)
				{
					return;
				}

				Player->UpdateCaption(Data, 1.0f);

				PC->bPrimaryOldMop = true;
			}
		}
	}
}

void AMop::SpawnTool()
{
	
}

void AMop::DrawOutline(bool Draw)
{
	SkeletalMesh->SetRenderCustomDepth(Draw);
}

void AMop::SetCollisionType(const FName& InType)
{
	if (SkeletalMesh)
	{
		SkeletalMesh->SetCollisionProfileName(InType);
	}
}

void AMop::C2S_DecreaseDurability_Implementation(ATextureDecal* InDecal, APawn* Character)
{
	Durability -= Reduction;
	S2A_SetMopDurability(Durability);
	S2A_SetMopMaterial(Durability);

	if (ObjectOwner)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		AMop* FPSMop = Cast<AMop>(Player->FPSOwnedActor);
		if (FPSMop)
		{
			FPSMop->S2A_SetMopDurability(Durability);
			FPSMop->S2A_SetMopMaterial(Durability);
		}
	}

	InDecal->C2S_SetAlphaTextureAndOpacity(Character);
}

void AMop::S2A_SetMopDurability_Implementation(int32 InDurability)
{
	Durability = InDurability;	
}

void AMop::CreateDecal(FVector ImpactPoint, FVector Normal)
{
	//UE_LOG(LogTemp, Warning, TEXT(" S2A_CreateDecal_Implementation Create Decal"));
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATextureDecal* GeneratedDecal = GetWorld()->SpawnActor<ATextureDecal>(DecalClass, ImpactPoint, Normal.Rotation(), SpawnParam);

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

		GeneratedDecal->S2A_SetSize(DecalSize);
		GeneratedDecal->S2A_SetBoxExtent(DecalSize);
		GM->SpawnDecal.Add(GeneratedDecal);
	}
}

void AMop::S2A_SetMopMaterial_Implementation(int32 InDurability)
{
	SetMopMaterial(InDurability);
}

void AMop::S2A_SwingSoundPlay_Implementation()
{
	if (SwingSound && SwingSoundAudio)
	{
		SwingSoundAudio->Play();
	}
}

void AMop::S2A_SetFPSMop_Implementation(AActor* InPlayer, int32 InDurability, int32 InMaxDurability, int32 InCleanDurability, int32 InDirtyDurability)
{
	ObjectOwner = InPlayer;
	Durability = InDurability;
	MaxDurability = InMaxDurability;
	CleanDurability = InCleanDurability;
	DirtyDurability = InDirtyDurability;

	SetMopMaterial(Durability);
}

void AMop::SetMopMaterial(int32 InDurability)
{
	if (InDurability >= CleanDurability)
	{
		static FString HeadPath = TEXT("/Game/Materials/interactive_object/Dirty_object_material/mop/MI_Mop_Head_v1.MI_Mop_Head_v1");
		HeadMI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *HeadPath));

		static FString StickPath = TEXT("/Game/Materials/interactive_object/Dirty_object_material/mop/MI_Mop_Stick_v1.MI_Mop_Stick_v1");
		StickMI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *StickPath));
	}
	else if (InDurability <= DirtyDurability)
	{
		static FString HeadPath = TEXT("/Game/Materials/interactive_object/Dirty_object_material/mop/MI_Mop_Head_v3.MI_Mop_Head_v3");
		HeadMI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *HeadPath));

		static FString StickPath = TEXT("/Game/Materials/interactive_object/Dirty_object_material/mop/MI_Mop_Stick_v3.MI_Mop_Stick_v3");
		StickMI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *StickPath));
	}
	else
	{
		static FString HeadPath = TEXT("/Game/Materials/interactive_object/Dirty_object_material/mop/MI_Mop_Head_v2.MI_Mop_Head_v2");
		HeadMI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *HeadPath));

		static FString StickPath = TEXT("/Game/Materials/interactive_object/Dirty_object_material/mop/MI_Mop_Stick_v2.MI_Mop_Stick_v2");
		StickMI = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), this, *StickPath));
	}

	if (HeadMI)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeadMI is Exist"));
		SkeletalMesh->SetMaterial(1, HeadMI);
	}

	if (StickMI)
	{
		UE_LOG(LogTemp, Warning, TEXT("StickMI is Exist"));
		SkeletalMesh->SetMaterial(0, StickMI);
	}
}
