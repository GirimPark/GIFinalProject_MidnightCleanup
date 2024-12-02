// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAwayPoint.h"

// Sets default values
ARunAwayPoint::ARunAwayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PointType = EPointType::RunAway;
}

// Called when the game starts or when spawned
void ARunAwayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}



