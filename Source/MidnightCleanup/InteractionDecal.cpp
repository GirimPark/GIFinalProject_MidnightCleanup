// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionDecal.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Net/UnrealNetwork.h"

AInteractionDecal::AInteractionDecal()
{
	ObjectType = EObjectType::ObjectType_Static;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	RootComponent = Box;
	
	StaticMesh->SetupAttachment(RootComponent);

	Box->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Box->SetCollisionProfileName(TEXT("Decal"));
	Box->SetSimulatePhysics(false);
	
	StaticMesh->SetSimulatePhysics(false);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Box);

	SortOrder = 0;
}

void AInteractionDecal::BeginPlay()
{
	Super::BeginPlay();
	
	MI = Decal->CreateDynamicMaterialInstance();
	S2A_SetSortOrder(SortOrder);
}

void AInteractionDecal::InterAction(APawn* Character)
{
}

void AInteractionDecal::DrawOutline(bool Draw)
{
	
}

void AInteractionDecal::HintDraw(bool Draw)
{
	if (Draw)
	{
		MI->SetScalarParameterValue(TEXT("luminous"), 1);
	}
	else
	{
		MI->SetScalarParameterValue(TEXT("luminous"), 0);
	}
}

void AInteractionDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractionDecal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractionDecal, MinLegnth);
	DOREPLIFETIME(AInteractionDecal, SortOrder);
	DOREPLIFETIME(AInteractionDecal, Durability);
}

const int32 AInteractionDecal::GetSortOrder()
{
	return SortOrder;
}

void AInteractionDecal::S2A_SetSortOrder_Implementation(int32 InSortOrder)
{
	SortOrder = InSortOrder;
	Decal->SortOrder = SortOrder;
}

void AInteractionDecal::S2A_SetBoxExtent_Implementation(FVector InSize)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetBoxExtent"));
	if (Box)
	{
		Box->SetBoxExtent(InSize);
	}
}
