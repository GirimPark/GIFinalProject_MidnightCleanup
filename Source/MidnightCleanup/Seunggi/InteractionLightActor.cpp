// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionLightActor.h"
#include "Components\LightComponent.h"
#include "InteractionSwitch.h"

// Sets default values
AInteractionLightActor::AInteractionLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	
}

void AInteractionLightActor::BeginPlay()
{
	Super::BeginPlay();

}

void AInteractionLightActor::SetLightComponent(ULightComponentBase* LightComponentType)
{
	LightComponent = LightComponentType;
	LightIntensity = LightComponentType->Intensity;
}

void AInteractionLightActor::Initialize()
{
	if (Switch)
	{
		if (!Switch->bIsLighting)
		{
			Cast<ULightComponent>(LightComponent)->SetIntensity(0);
			StaticMesh->SetScalarParameterValueOnMaterials(FName("emissive"), 0.0f);
		}
	}
}


