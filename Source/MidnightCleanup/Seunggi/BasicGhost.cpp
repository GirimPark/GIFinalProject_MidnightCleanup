// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGhost.h"
#include "Components\SkeletalMeshComponent.h"
#include "Components\SphereComponent.h"
#include "Components\SplineComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "../InteractionTrash.h"
#include "InteractionSwitch.h"
#include "InteractionDoor.h"
#include "../InteractionBox.h"

// Sets default values
ABasicGhost::ABasicGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(1000);

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABasicGhost::ProcessBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABasicGhost::ProcessEndOverlap);
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
}

// Called when the game starts or when spawned
void ABasicGhost::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ABasicGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetActorLocation() + FVector(0, 0, 220));
	}
}

// Called to bind functionality to input
void ABasicGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicGhost::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Type == EGhostType::Trash&&State==EGhostState::Patrol)
	{
		AInteractionPickUpObject* Object = Cast<AInteractionPickUpObject>(OtherActor);
		AInteractionBox* BoxObject = Cast<AInteractionBox>(OtherActor);
		if(BoxObject)
		{
			if(BoxObject->TrashNum>0 && CanBoxSteal()&&BoxObject->ObjectOwner==nullptr)
			{
				UE_LOG(LogTemp,Warning,TEXT("Box"))
				DetectedObject.Add(BoxObject);
			}
		}
		else if (Object && CanInteraction()&&Object->ObjectOwner==nullptr&&Object->PickUpObjectType!=EPickUpObjectType::PickUpObjectType_Furniture&& Object->PickUpObjectType != EPickUpObjectType::PickUpObjectType_Bucket)
		{
			UE_LOG(LogTemp, Warning, TEXT("Trash"))
			DetectedObject.Add(Object);
		}
	}
	else if (Type == EGhostType::Light)
	{
		AInteractionSwitch* Object = Cast<AInteractionSwitch>(OtherActor);
		if (Object)
		{
			if(Object->bIsLighting && CanInteraction())
			{
				FString name = Object->GetName();
				DetectedObject.Add(Object);
			}
		}
	}
	else if (Type == EGhostType::Door)
	{
		AInteractionDoor* Object = Cast<AInteractionDoor>(OtherActor);
		if (Object)
		{
			if (Object->bIsOpened && CanInteraction())
			{
				FString name = Object->GetName();
				DetectedObject.Add(Object);
			}
		}
	}
}

void ABasicGhost::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABasicInteractionObject* Object = Cast<ABasicInteractionObject>(OtherActor);
	if (Object)
	{
		DetectedObject.Remove(Object);
	}
}

bool ABasicGhost::CanInteraction()
{
	Probability = FMath::Clamp(Probability, 0, 100);

	// Generate a random float in the range [0.0, 1.0]
	float RandomValue = FMath::RandRange(1, 100);
	// Return true if the random value is less than or equal to the probability
	return RandomValue < Probability;
}

bool ABasicGhost::CanBoxSteal()
{
	BoxProbability = FMath::Clamp(BoxProbability, 0, 100);

	// Generate a random float in the range [0.0, 1.0]
	float RandomValue = FMath::RandRange(1, 100);
		// Return true if the random value is less than or equal to the probability
	return RandomValue < BoxProbability;
}

