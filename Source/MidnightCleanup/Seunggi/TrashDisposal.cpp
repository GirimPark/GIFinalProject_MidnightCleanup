// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashDisposal.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "InteractionPickUpObject.h"
#include "InteractionWoodenPlank.h"

// Sets default values
ATrashDisposal::ATrashDisposal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("Interactable"));
	RootComponent = Box;

	Box->OnComponentBeginOverlap.AddDynamic(this, &ATrashDisposal::ProcessBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ATrashDisposal::ProcessEndOverlap);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ATrashDisposal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrashDisposal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrashDisposal::ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInteractionPickUpObject* Object = Cast<AInteractionPickUpObject>(OtherActor);
	if (Object)
	{
		if (Object->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Trash || Object->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box || Object->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Bucket)
		{
			if (Cast<AInteractionWoodenPlank>(Object))
			{
				return;
			}
			FTimerDelegate TimerDel;
			TimerDel = FTimerDelegate::CreateUObject(this, &ATrashDisposal::DestroyObject, OtherActor);
			//TimerDel.BindUFunction(this, FName("PrintLog"), OtherActor->GetFName().ToString());

			GetWorldTimerManager().SetTimer(Object->TimerHandle, TimerDel, 1.0f, false, DestroyTime);
			UE_LOG(LogTemp, Warning, TEXT("Destroy timer set"))
		}
	}
}

void ATrashDisposal::ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInteractionPickUpObject* Object = Cast<AInteractionPickUpObject>(OtherActor);
	if (Object)
	{
		GetWorldTimerManager().ClearTimer(Object->TimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Destroy timer clear"))
	}
	
}

void ATrashDisposal::DestroyObject(AActor* Actor)
{
	if(HasAuthority()&&Actor)
	{
		bool destroyed = GetWorld()->DestroyActor(Actor);
		UE_LOG(LogTemp, Warning, TEXT("Destroy Object : %d"), destroyed);
	}
}

void ATrashDisposal::PrintLog(FString log)
{
	UE_LOG(LogTemp, Warning, TEXT("name"));
}

void ATrashDisposal::C2S_DestroyObject_Implementation(AActor* Actor)
{
	DestroyObject(Actor);
}

