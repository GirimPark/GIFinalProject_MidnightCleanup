// Fill out your copyright notice in the Description page of Project Settings.


#include "IndicatorVolumeBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerCharacter.h"
#include "Seunggi/InteractionPickUpObject.h"

// Sets default values
AIndicatorVolumeBase::AIndicatorVolumeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	if (Box)
	{
		Box->SetupAttachment(RootComponent);
	}
		
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	if (Widget)
	{
		Widget->SetupAttachment(Box);
	}
}

// Called when the game starts or when spawned
void AIndicatorVolumeBase::BeginPlay()
{
	Super::BeginPlay();

	if (Widget)
	{
		Widget->SetVisibility(false);
	}

	if (Box)
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &AIndicatorVolumeBase::IndicatorBeginOverlap);
		Box->OnComponentEndOverlap.AddDynamic(this, &AIndicatorVolumeBase::IndicatorEndOverlap);
	}
}

void AIndicatorVolumeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Box)
	{
		return;
	}

	TArray<AActor*> Players;
	Box->GetOverlappingActors(Players, APlayerCharacter::StaticClass());
	for (AActor* Temp : Players)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Temp);
		if (Player)
		{
			Player->CheckIndicator(this);
		}
	}
}

void AIndicatorVolumeBase::IndicatorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		Player->S2C_EnableIndicator(this);
	}
}

void AIndicatorVolumeBase::IndicatorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		Player->S2C_DisableIndicator(this);
	}
}

bool AIndicatorVolumeBase::IsVisibleWidget()
{
	if (Widget)
	{
		return Widget->IsVisible();
	}
	else
	{
		return false;
	}
}

