// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskObject.h"
#include "../PlayerCharacter.h"
#include "Camera\CameraComponent.h"
#include "Kismet\KismetMathLibrary.h"
#include "Components\BoxComponent.h"
#include "Net\UnrealNetwork.h"
#include "../InGamePS.h"
#include "Kismet\GameplayStatics.h"
#include "Sound\SoundCue.h"

AMaskObject::AMaskObject()
{
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box->SetupAttachment(StaticMesh);
}

void AMaskObject::BeginPlay()
{
	Super::BeginPlay();
	TargetLocation = GetActorLocation();
	StaticMesh->SetSimulatePhysics(true);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AMaskObject::ProcessBeginOverlap);
}

void AMaskObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAttach)
	{
		/*if(HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Server : %d"), GetIsReplicated());
		}
		else {
			
		}*/
		return;
	}
	FVector ResultPosition;
	FRotator ResultRotation;

	//ResultPosition = FMath::VInterpTo(GetActorLocation() , TargetLocation, GetWorld()->GetDeltaSeconds(), 25.0f);
	//ResultRotation = FMath::RInterpTo(GetActorRotation(), TargetRotator, GetWorld()->GetDeltaSeconds(), 15.0f);
	//SetActorLocation(ResultPosition);
	//SetActorRotation(TargetRotator);
	
}

void AMaskObject::InterAction(APawn* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player)
	{
		FRotator Rotation = Player->GetActorRotation() + FRotator(0, 90, 0);
		FVector Location = Player->GetActorLocation() + Player->Camera->GetRelativeLocation()+(UKismetMathLibrary::GetForwardVector(FRotator(Rotation.Pitch,Rotation.Yaw-90, Rotation.Roll))*30);
		S2A_SetTargetLocationAndRotation(Location, Rotation);
		bActivate = true;
		//S2A_SoundPlay();
	}
	if (bAttach)
	{
		Destroy();
	}
}

void AMaskObject::DrawOutline(bool Draw)
{
	StaticMesh->SetRenderCustomDepth(Draw);
}

void AMaskObject::UpdateToolTip(APlayerCharacter* Player)
{

}

void AMaskObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaskObject, AttachPlayer);
	DOREPLIFETIME(AMaskObject, FPSMask);
	DOREPLIFETIME(AMaskObject, bActivate);
	DOREPLIFETIME(AMaskObject, bAttach);
}

void AMaskObject::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player&&bActivate)
	{
		//if(HasAuthority())
		{
			C2S_AttachToPlayer(Player);
		}
		
		//if (HasAuthority())
		//{
		//	if(bActivate)
		//	{
		//		AttachPlayer = Player;
		//		C2S_AttachToPlayer(Player);
		//	}
		//}
	}
}

void AMaskObject::OnRep_AttachToPlayer()
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
	);
	if(AttachPlayer)
	{
		
		if (AttachPlayer->IsLocallyControlled())
		{
			if (FPSMask)
			{
				FPSMask->StaticMesh->SetSimulatePhysics(false);
				FPSMask->AttachToComponent(AttachPlayer->FPSMesh, AttachmentRules, FName("Mask"));
				StaticMesh->SetVisibility(false);
			}
		}
		else
		{
			AttachToComponent(AttachPlayer->GetMesh(), AttachmentRules, FName("Mask"));
			if (FPSMask)
			{
				FPSMask->StaticMesh->SetVisibility(false);
			}
		}
	}
}

void AMaskObject::C2S_AttachToPlayer_Implementation(APlayerCharacter* Player)
{
	//FActorSpawnParameters SpawnParam;
	//SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//FTransform Transform = FTransform(GetActorLocation());
	//FPSMask = GetWorld()->SpawnActor<AMaskObject>(this->GetClass(), Transform, SpawnParam);
	//OnRep_AttachToPlayer();
	bActivate = false;
	bAttach = true;
	StaticMesh->SetSimulatePhysics(false);
	//SetReplicates(false);
	

	S2A_AttachToPlayer(Player);
	Player->S2A_AddActorsToIgnore(this);

	Player->UpdateEnableTriggerCount();
}


void AMaskObject::S2A_AttachToPlayer_Implementation(APlayerCharacter* Player)
{
	StaticMesh->SetSimulatePhysics(false);
	
	bActivate = false;
	bAttach = true;
	SetReplicates(false);
	if (Player->IsLocallyControlled())
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::SnapToTarget,
			true
		);
		
		SetActorRotation(FRotator(0,270,0));
		AttachToComponent(Player->CameraSocket, AttachmentRules);
		UGameplayStatics::PlaySound2D(Player, ObjectSound);
	}
	else
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			true
		);
		AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Mask"));
	}
	//if (Player->IsLocallyControlled())
	//{
	//	if (FPSMask)
	//	{
	//		FPSMask->Box->SetSimulatePhysics(false);
	//		FPSMask->AttachToComponent(Player->FPSMesh, AttachmentRules,FName("Mask"));
	//		StaticMesh->SetVisibility(false);
	//	}
	//}
	//else
	//{
	//	AttachToComponent(Player->GetMesh(), AttachmentRules, FName("Mask"));
	//	if (FPSMask)
	//	{
	//		FPSMask->StaticMesh->SetVisibility(false);
	//	}
	//}
}

void AMaskObject::S2A_SetTargetLocationAndRotation_Implementation(FVector Location, FRotator Rotator)
{
	TargetLocation = Location;
	TargetRotator = Rotator;

	FVector ImpulseVector = (TargetLocation - GetActorLocation());
	ImpulseVector.Normalize();
	StaticMesh->AddImpulse(ImpulseVector * 1000);

	SetActorRotation(Rotator);
}

void AMaskObject::S2A_EnableEffect_Implementation()
{
	EnableEffect();
}
