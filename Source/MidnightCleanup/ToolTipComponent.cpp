// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolTipComponent.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UToolTipComponent::UToolTipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UToolTipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UToolTipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FToolTipData* UToolTipComponent::GetToolTipByID(FName ID)
{
	const FString ContextString(TEXT("Tooltip Context"));
	return ToopTipDataTable->FindRow<FToolTipData>(ID, ContextString, true);
}


