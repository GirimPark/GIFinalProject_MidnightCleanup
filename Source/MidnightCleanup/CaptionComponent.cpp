// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptionComponent.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UCaptionComponent::UCaptionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCaptionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCaptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FCaptionData* UCaptionComponent::GetCaptionByID(FName ID)
{
	const FString ContextString(TEXT("Caption Context"));
	return CaptionDataTable->FindRow<FCaptionData>(ID, ContextString, true);
}

