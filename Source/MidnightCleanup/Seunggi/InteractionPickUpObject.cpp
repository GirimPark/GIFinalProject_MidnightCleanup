// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPickUpObject.h"
#include "Components\BoxComponent.h"
#include "BasicGhost.h"
#include "../PlayerCharacter.h"
#include "../BasicTool.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet\GameplayStatics.h"
#include "Sound\SoundCue.h"
#include "Components/AudioComponent.h"
#include "InGameGS.h"
#include "InGameGM.h"
#include "PotObject.h"
#include "../PC.h"
#include "Components\SphereComponent.h"

AInteractionPickUpObject::AInteractionPickUpObject()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicateMovement(true);
	//Box->DestroyComponent();
	/*Box->SetSimulatePhysics(true);
	Box->SetUseCCD(true);
	StaticMesh->SetUseCCD(true);
	Box->bHasPerInstanceHitProxies = true;*/
	//Box->DestroyComponent();
	//SetRootComponent(StaticMesh);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetUseCCD(true);
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(10);


	ObjectType = EObjectType::ObjectType_PickUp;

	//Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractionPickUpObject::ProcessBeginOverlap);
}


void AInteractionPickUpObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
		StaticMesh->OnComponentHit.AddDynamic(this, &AInteractionPickUpObject::BoxBeginHit);
	if (DropSound)
	{
		DropSoundAudio = NewObject<UAudioComponent>(this);
		DropSoundAudio->SetupAttachment(RootComponent);
		DropSoundAudio->SetSound(DropSound);  // DropSound는 사운드 큐로 설정
		DropSoundAudio->SetIsReplicated(true);
		DropSoundAudio->RegisterComponent();
		DropSoundAudio->Stop();
	}
	if (StealSound)
	{
		StealSoundAudio = NewObject<UAudioComponent>(this);
		StealSoundAudio->SetupAttachment(RootComponent);
		StealSoundAudio->SetSound(StealSound);  // DropSound는 사운드 큐로 설정
		StealSoundAudio->SetIsReplicated(true);
		StealSoundAudio->RegisterComponent();
		StealSoundAudio->Stop();
	}
}

void AInteractionPickUpObject::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Cast<APotObject>(this))
		return;
	if (Player)
	{
		if (!Player->bIsHeld)
		{
			SetObjectOwner(Character);
			PickUp(Character);
			bIsCleaning = false;
			OnRep_SetCleaningState();
		}
	}
	else if (!Player)
	{
		PickUp(Character);
		bIsCleaning = false;
		OnRep_SetCleaningState();
	}
}

void AInteractionPickUpObject::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AInteractionPickUpObject::UpdateToolTip(APlayerCharacter* Player)
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



void AInteractionPickUpObject::PickUp_Implementation(APawn* Character)
{
	//Box->SetSimulatePhysics(false);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	ABasicGhost* GhostCharacter = Cast<ABasicGhost>(Character);
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true
	);

	if (PlayerCharacter)
	{
		
		PlayerCharacter->AddInventoryCurrentIndex(this);

		//if (PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box)
		//{
		//	PlayerCharacter->bIsHeldBox = true;
		//
		//	//PickUpTrash(Character);
		//	PickUpObject(PlayerCharacter);
		//}
		//AttachToComponent(PlayerCharacter->GetMesh(), AttachmentRules, FNam("Box"));
		//StaticMesh->SetSimulatePhysics(true);
		DrawOutline(false);
		PickUpObject(PlayerCharacter);
	}

	else if (GhostCharacter)
	{
		PickUpObject(GhostCharacter);
		//Box->SetSimulatePhysics(false);
	}

}

void AInteractionPickUpObject::Drop_Implementation(APawn* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	FDetachmentTransformRules DetachmentRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		false
	);
	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveInventoryIndex(PlayerCharacter->CurrentInventoryIndex);
		//DropObject(PlayerCharacter);
	}

	//DetachFromActor(DetachmentRules);
	DropProcess(Character);
	//APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	//ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	//if (Player)
	//{
	//	Player->PhysicsHandle->ReleaseComponent();
	//
	//	if (Player->PhysicsHandle->GetGrabbedComponent())
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("GrabbedComponent : %s"), *Player->PhysicsHandle->GetGrabbedComponent()->GetName());
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("GrabbedComponent is Empty"));
	//	}
	//	
	//}
	//else if (Ghost)
	//{
	//	Ghost->PhysicsHandle->ReleaseComponent();
	//}
}

void AInteractionPickUpObject::DropPrevious_Implementation(APawn* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	FDetachmentTransformRules DetachmentRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		false
	);

	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveInventoryIndex(PlayerCharacter->PreviousInventoryIndex);


		if (PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box)
		{
			PlayerCharacter->bIsHeldBox = false;
		}
	}

	DropProcess(Character);
}

void AInteractionPickUpObject::PickUpObject(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	if (Player)
	{
		switch (PickUpObjectType)
		{
		case EPickUpObjectType::PickUpObjectType_Trash:
		{
			Player->bIsHeldTrash = true;
		}
		break;
		case EPickUpObjectType::PickUpObjectType_Box:
		{
			Player->bIsHeldBox = true;
		}
		break;
		case EPickUpObjectType::PickUpObjectType_Bucket:
		{
			Player->bIsHeldBucket = true;
		}
		break;
		case EPickUpObjectType::PickUpObjectType_WoodenPlank:
		{
			Player->bIsHeldWoodenPlank = true;
		}
		break;
		}

		StaticMesh->SetSimulatePhysics(true);
		FVector Grab = GetActorTransform().TransformPosition(GrabLocation);

		FVector location = Grab;//StaticMesh->GetSocketLocation(FName("Socket"));
		Player->PhysicsHandle->GrabComponentAtLocationWithRotation(StaticMesh, NAME_None, location, GetActorRotation());
		//UE_LOG(LogTemp, Warning, TEXT("Pick up Object"))
	}
	else if (Ghost)
	{
		StaticMesh->SetSimulatePhysics(true);
		Ghost->PhysicsHandle->GrabComponentAtLocationWithRotation(StaticMesh, NAME_None, GetActorLocation(), GetActorRotation());
	}
}

void AInteractionPickUpObject::PickUpTool(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	if (Player)
	{
		Player->PhysicsHandle->GrabComponentAtLocationWithRotation(StaticMesh, NAME_None, GetActorLocation(), GetActorRotation());
	}
	else if (Ghost)
	{
		Ghost->PhysicsHandle->GrabComponentAtLocationWithRotation(StaticMesh, FName("Tool"), GetActorLocation(), GetActorRotation());
	}
}

void AInteractionPickUpObject::SetObjectOwner(APawn* Character)
{
	if (ObjectOwner && ObjectOwner != Character)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(ObjectOwner);
		if (Player)
		{
			Player->DropAbsolutely();

			APlayerCharacter* NewPlayer = Cast<APlayerCharacter>(Character);
			NewPlayer->C2S_UpdateStealCount();

		}
	}

	S2A_SetObjectOwner(Character);
}

void AInteractionPickUpObject::S2A_SetObjectOwner_Implementation(APawn* Character)
{
	ObjectOwner = Character;
	if (nullptr == Character)
	{
		return;
	}
	LastOwner = Character;
}

void AInteractionPickUpObject::S2A_SetDuplicateActor_Implementation(AInteractionPickUpObject* Actor)
{
	DuplicateActor = Actor;
}

void AInteractionPickUpObject::S2A_SetOverlapEvents_Implementation(bool State)
{
	//Box->SetGenerateOverlapEvents(State);
}

void AInteractionPickUpObject::S2A_PlayDropSound_Implementation()
{
	if (DropSound && DropSoundAudio)
	{
		DropSoundAudio->Play();
	}
}

void AInteractionPickUpObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionPickUpObject, ObjectOwner);
	DOREPLIFETIME(AInteractionPickUpObject, DuplicateActor);
	DOREPLIFETIME(AInteractionPickUpObject, bIsCleaning);
	DOREPLIFETIME(AInteractionPickUpObject, LastOwner);
}

void AInteractionPickUpObject::BoxBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ObjectOwner)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("impulse size : %f"), NormalImpulse.Size());
	if (NormalImpulse.Size() > 10)
	{
		if (DropSound && DropSoundAudio)
		{
			S2A_PlayDropSound();
			//DropSoundAudio->Play();
			//if (!DropSoundAudio->IsPlaying())
			//{
			//	DropSoundAudio->Play();
			//	UE_LOG(LogTemp, Warning, TEXT("Playing sound"));
			//}
			//else
			//{
			//	DropSoundAudio->Stop();
			//	DropSoundAudio->Play();
			//	UE_LOG(LogTemp, Warning, TEXT("Sound is already playing"));
			//}
		}
	}
}

void AInteractionPickUpObject::OnRep_SetCleaningState()
{
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CountObject();
	}
}

void AInteractionPickUpObject::DropObject(APawn* Character)
{
	S2A_SetObjectOwner(nullptr);
	Drop(Character);
}

void AInteractionPickUpObject::DropPreviousObject(APawn* Character)
{
	S2A_SetObjectOwner(nullptr);
	DropPrevious(Character);
}

void AInteractionPickUpObject::DropProcess(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	ABasicGhost* Ghost = Cast<ABasicGhost>(Character);
	if (Player)
	{
		switch (PickUpObjectType)
		{
		case EPickUpObjectType::PickUpObjectType_Trash:
		{
			Player->bIsHeldTrash = false;
		}
		break;
		case EPickUpObjectType::PickUpObjectType_Box:
		{
			Player->bIsHeldBox = false;
		}
		break;
		case EPickUpObjectType::PickUpObjectType_Bucket:
		{
			Player->bIsHeldBucket = false;
		}
		break;
		case EPickUpObjectType::PickUpObjectType_WoodenPlank:
		{
			Player->bIsHeldWoodenPlank = false;
		}
		break;
		}

		Player->PhysicsHandle->ReleaseComponent();
		Player->OwnedActor = nullptr;
	}
	else if (Ghost)
	{
		Ghost->PhysicsHandle->ReleaseComponent();
	}
}
