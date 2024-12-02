// Fill out your copyright notice in the Description page of Project Settings.


#include "LargePickUpObject.h"
#include "../PlayerCharacter.h"
#include "Components\BoxComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

void ALargePickUpObject::InterAction(APawn* Character)
{
	S2A_PickUp(Character);
}

void ALargePickUpObject::DropProcess(APawn* Character)
{
	if (Player1 == Character)
	{
		Player1->PhysicsHandle->ReleaseComponent();
		Player1 = nullptr;
		SetOwner(Player2);
	}
	else if (Player2 == Character)
	{
		Player2->PhysicsHandle->ReleaseComponent();
		Player2 = nullptr;
	}
}

void ALargePickUpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if(Player1&&Player2)
	{
		FVector NewLocation = (Player1->GetActorLocation() + Player2->GetActorLocation()) / 2;
		SetActorLocation(NewLocation);
	}*/
}

void ALargePickUpObject::S2A_PickUp_Implementation(APawn* Character)
{
	if (Player1)
	{
		Player2 = Cast<APlayerCharacter>(Character);
		FVector location = StaticMesh->GetSocketLocation(FName("Socket2"));
		
		//Player2->PhysicsHandle->GrabComponentAtLocation(Box, NAME_None, location);
		
		FString name = Player1->GetName();
		FString name2 = Player2->GetName();

		UE_LOG(LogTemp, Warning, TEXT("Player1 : %s,Player2 : %s"), *name, *name2);

		
	}
	else 
	{
		Player1 = Cast<APlayerCharacter>(Character);
		SetOwner(Player1);
		FVector location = StaticMesh->GetSocketLocation(FName("Socket"));
		//Player1->PhysicsHandle->GrabComponentAtLocation(Box, FName("Socket"), location);		
	}
}


