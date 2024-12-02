// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostSpawnPoint.h"
#include "Components\SplineComponent.h"

AGhostSpawnPoint::AGhostSpawnPoint()
{
	SpawnType = ESpawnType::Ghost;
	PatrolPoints = CreateDefaultSubobject<USplineComponent>(TEXT("Patrol Points"));
	RootComponent = PatrolPoints;
}

void AGhostSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}
