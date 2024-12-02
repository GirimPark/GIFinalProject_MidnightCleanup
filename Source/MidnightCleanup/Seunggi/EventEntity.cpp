// Fill out your copyright notice in the Description page of Project Settings.


#include "EventEntity.h"

// Sets default values
AEventEntity::AEventEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

