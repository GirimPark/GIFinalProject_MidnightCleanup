// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"
#include "BasicGhost.h"
#include "EngineUtils.h"
#include "Components\SplineComponent.h"
#include "ObjectSpawnPoint.h"
#include "../InteractionBox.h"
#include "Components\BoxComponent.h"
#include "../TextureDecal.h"
#include "../OpacityDecal.h"
#include "../InteractionTrash.h"
#include "../InteractionFurniture.h"
#include "../InteractionBucket.h"
#include "InteractionWoodenPlank.h"
#include "../CleanProgressMonitor.h"
#include "../LimitTimeMonitor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../InteractionFurniture.h"
#include "InteractionWoodenPlank.h"
#include "../OpacityDecal.h"
#include "../TextureDecal.h"
#include "InGameGS.h"
#include "Props.h"
#include "Algo/Count.h"
#include "../GI.h"
#include "GameFramework\PlayerStart.h"
#include "../InGamePS.h"
#include "Camera\CameraActor.h"
#include "../PC.h"

AInGameGM::AInGameGM()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInGameGM::BeginPlay()
{
	Super::BeginPlay();

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &AInGameGM::OnLevelLoaded);

	CurrentPowerLevel = 0;
	CurrentActiveEntityNum = 0;
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->C2S_SendUserInfo();
	}

	InitCountObject();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AInGameGM::DecreaseTime, 1.0f, true, 1.0f);

	UWorld* World = GetWorld();

	for (TActorIterator<APointBase> It(World); It; ++It)
	{
		ASpawnPoint* sp = Cast<ASpawnPoint>(*It);
		if (sp)
		{
			for (FEntityPowerLevel& Entity : Entities)
			{
				if (Entity.EntityType == EEntityType::Ghost && sp->SpawnType == ESpawnType::Ghost)
				{
					Entity.SpawnPoint.Add(sp);
				}
			}
		}
		else
		{
			RunAwayPoints.Add(*It);
		}
	}
	for (TActorIterator<ABasicEntity> It(World); It; ++It)
	{
		for (int i = 0; i < Entities.Num(); i++)
		{
			if (It->GetClass() == Entities[i].Entity)
			{
				Entities[i].SpawnPoint.Add(*It);
			}
		}
		
	}
	OutTruckCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));

	if (CleanProgressMonitorClass)
	{
		FVector Location = FVector(-2457.520048, -493.596343, 447.110906);
		FRotator Rotation = FRotator(0, 180, 0);
		FVector Scale = FVector(0.15, 0.122828, 0.107198);
		FTransform Transform;
		Transform.SetTranslation(Location);
		Transform.SetRotation(Rotation.Quaternion());
		Transform.SetScale3D(Scale);
		C2S_SpawnCleanProgressMonitor(CleanProgressMonitorClass, Transform);
	}

	if (LimitTimesMonitorClass)
	{
		FVector Location = FVector(-2597.0, -694.974559, 441.308408);
		FRotator Rotation = FRotator(0.f, 90.f, 0.f);
		FVector Scale = FVector(0.1f, 0.1f, 0.1f);
		FTransform Transform;
		Transform.SetTranslation(Location);
		Transform.SetRotation(Rotation.Quaternion());
		Transform.SetScale3D(Scale);
		C2S_SpawnLimitTimeMonitor(LimitTimesMonitorClass, Transform);
	}
}

void AInGameGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInGameGM::SpawnAICharacter_Implementation(TSubclassOf<ABasicGhost> Ghost, AGhostSpawnPoint* SpawnPoint)
{
	if (Ghost)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(SpawnPoint->GetActorLocation());
		ABasicGhost* SpawnedGhost = GetWorld()->SpawnActor<ABasicGhost>(Ghost, Transform, SpawnParam);
		SpawnedGhost->PatrolPoints = SpawnPoint->PatrolPoints;
		SpawnAI.Add(SpawnedGhost);
	}
}

void AInGameGM::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}

void AInGameGM::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);
	APlayerController* NewPlayer = Cast<APlayerController>(C);
	if (NewPlayer)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

		if (PlayerStarts.Num() > 0)
		{
			if(Index < PlayerStarts.Num())
			// Choose the first PlayerStart for spawning (can be customized)
			{
				APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[Index]);

				if (PlayerStart)
				{
					FActorSpawnParameters SpawnParam;
					SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					// Get the PlayerStart transform (location and rotation)
					FTransform SpawnTransform = PlayerStart->GetActorTransform();

					// Spawn the character at the PlayerStart location
					//UE_LOG(LogTemp, Warning, TEXT("Post Login"));


					AInGamePS* PS = NewPlayer->GetPlayerState<AInGamePS>();
					if (PS)
					{
						TSubclassOf<APlayerCharacter> SelectedCharacter = PS->PlayerCharacter;
						if(SelectedCharacter)
						{
							// Assign the selected character to the player
							NewPlayer->GetPawn()->Destroy(); // If they already have a pawn
							APlayerCharacter* NewCharacter = GetWorld()->SpawnActor<APlayerCharacter>(SelectedCharacter, SpawnTransform, SpawnParam);
							NewPlayer->Possess(NewCharacter);
						}
					}
				}
				Index++;
			}
		}
	}
}

void AInGameGM::ChangeLevel(FString InLevelName)
{
	GetWorld()->ServerTravel(InLevelName);
}

void AInGameGM::CanSpawnEntity(FEntityPowerLevel Object, int32 InTime)
{
	if (MaxPowerLevel - CurrentPowerLevel < Object.PowerLevel||CurrentActiveEntityNum>=MaxActiveEntityNum)
	{
		return;
	}
	else
	{
		int RandomNum = FMath::RandRange(0, 100);
		if (RandomNum <= Object.CurveFloat->GetFloatValue(InTime))
		{
			if (Object.Entity && Object.Entity->IsChildOf<ABasicGhost>())
			{
				if (Object.SpawnPoint.Num() > 0)
				{
					int index = FMath::RandRange(0, Object.SpawnPoint.Num() - 1);
					AGhostSpawnPoint* SpawnPoint = Cast<AGhostSpawnPoint>(Object.SpawnPoint[index]);
					if (SpawnPoint)
					{
						SpawnEntity(Object.Entity, SpawnPoint);
					}
					CurrentPowerLevel += Object.PowerLevel;
					CurrentActiveEntityNum++;
				}
			}
			else if (Object.EntityType == EEntityType::Event)
			{
				if (Object.SpawnPoint.Num() > 0)
				{
					int index = SelectEntity(Object.SpawnPoint);
					if(index!=-1)
					{
						ABasicEntity* Entity = Cast<ABasicEntity>(Object.SpawnPoint[index]);
						if (Entity)
						{
							Entity->C2S_SetActivate(true);
							CurrentPowerLevel += Object.PowerLevel;
							CurrentActiveEntityNum++;
							UE_LOG(LogTemp, Warning, TEXT("MAX Entity Num : %d ,Current Entity Num : %d"),MaxActiveEntityNum,CurrentActiveEntityNum);
						}
					}
				}
			}
		}
	}
	UWorld* World = GetWorld();

	for (TActorIterator<ABasicEntity> It(World); It; ++It)
	{
		if (It->bActivate)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Activate Entity : %s"), *It->GetName());
		}
	}
}

void AInGameGM::SpawnBox_Implementation(TSubclassOf<class AInteractionBox> Box, AObjectSpawnPoint* SpawnPoint)
{
	if (Box)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(SpawnPoint->GetActorLocation());
		AInteractionBox* SpawnedBox = GetWorld()->SpawnActor<AInteractionBox>(Box, Transform, SpawnParam);

		//SpawnedBox->Box->SetSimulatePhysics(true);
	}
}

void AInGameGM::C2S_SpawnCleanProgressMonitor_Implementation(TSubclassOf<class ACleanProgressMonitor> Monitor, FTransform Transform)
{
	if (Monitor)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CleanProgressMonitor = GetWorld()->SpawnActor<ACleanProgressMonitor>(Monitor, Transform, SpawnParam);
		//UE_LOG(LogTemp, Warning, TEXT("GM : SpawnWidget"));
		if (CleanProgressMonitor)
		{
			CleanProgressMonitor->Widget->SetDrawSize(FVector2D(1920.f, 1080.f));
			CleanProgressMonitor->Widget->SetWidgetSpace(EWidgetSpace::World);
		}
	}
}

void AInGameGM::C2S_SpawnLimitTimeMonitor_Implementation(TSubclassOf<class ALimitTimeMonitor> Monitor, FTransform Transform)
{
	if (Monitor)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		LimitTimeMonitor = GetWorld()->SpawnActor<ALimitTimeMonitor>(Monitor, Transform, SpawnParam);
		//UE_LOG(LogTemp, Warning, TEXT("GM : SpawnWidget"));
		if (LimitTimeMonitor)
		{
			LimitTimeMonitor->Widget->SetDrawSize(FVector2D(1920.f, 1080.f));
			LimitTimeMonitor->Widget->SetWidgetSpace(EWidgetSpace::World);
		}
	}
}

void AInGameGM::OpenInitGame()
{
	GetWorld()->ServerTravel(TEXT("L_InitGame"));
}

void AInGameGM::InitCountObject()
{
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		TArray<AActor*> Objects;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionFurniture::StaticClass(), Objects);
		GS->InitFurnitureCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionWoodenPlank::StaticClass(), Objects);
		GS->InitWoodCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionTrash::StaticClass(), Objects);
		GS->InitTrashCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATextureDecal::StaticClass(), Objects);
		GS->InitTextureDecalCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOpacityDecal::StaticClass(), Objects);
		GS->InitOpacityDecalCount = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProps::StaticClass(), Objects);
		GS->InitPropsCount = Objects.Num();

		CountObject();
		GS->OnRep_UpdateCount();
	}
}

void AInGameGM::CountObject()
{
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		TArray<AActor*> Objects;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionFurniture::StaticClass(), Objects);
		GS->BreakFurnitureNum = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionWoodenPlank::StaticClass(), Objects);
		GS->InitWoodCount = Objects.Num();
		int32 num = 0;
		for (AActor* actor : Objects)
		{
			AInteractionWoodenPlank* WoodenPlank = Cast<AInteractionWoodenPlank>(actor);
			if (WoodenPlank && WoodenPlank->bIsCleaning)
			{
				++num;
			}
		}
		GS->WoodenPlankNum = num;
	
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionTrash::StaticClass(), Objects);
		GS->TrashNum = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionBox::StaticClass(), Objects);
		GS->BoxNum = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionBucket::StaticClass(), Objects);
		GS->BucketNum = Objects.Num();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProps::StaticClass(), Objects);
		int32 PropsNum = 0;
		for (AActor* actor : Objects)
		{
			AProps* Props = Cast<AProps>(actor);
			if (Props && Props->bIsCleaning)
			{
				++PropsNum;
			}
		}
		GS->CleanedPropsNum = PropsNum;
		GS->OnRep_UpdateCount();
	}
}


void AInGameGM::SpawnEntity_Implementation(TSubclassOf<AActor> Entity, AGhostSpawnPoint* SpawnPoint)
{
	if (Entity->IsChildOf<ABasicGhost>())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Transform = FTransform(SpawnPoint->GetActorLocation());
		ABasicGhost* SpawnedGhost = GetWorld()->SpawnActor<ABasicGhost>(Entity, Transform, SpawnParam);
		SpawnedGhost->PatrolPoints = SpawnPoint->PatrolPoints;
	}
}

int AInGameGM::SelectEntity(TArray<AActor*> EntityList)
{
	TArray<int> IndexList;
	IndexList.Init(0, EntityList.Num());
	while (1)
	{
		int RandomIndex = FMath::RandRange(0, EntityList.Num() - 1);
		ABasicEntity* BE = Cast<ABasicEntity>(EntityList[RandomIndex]);
		if (BE&&BE->bActivate)
		{
			IndexList[RandomIndex] = 1;
		}
		if (IndexList[RandomIndex] == 0)
		{
			return RandomIndex;
		}
		if (Algo::Count(IndexList, 1) == EntityList.Num())
		{
			break;
		}
	}
	return -1;
}

void AInGameGM::UpdatePowerLevel(TSubclassOf<AActor> EntityClass)
{
	for (auto EntityElement : Entities)
	{
		if (EntityElement.Entity == EntityClass)
		{
			CurrentPowerLevel -= EntityElement.PowerLevel;
			CurrentActiveEntityNum--;
		}
	}
}

void AInGameGM::DecreaseTime()
{
	TArray<AActor*> Objects;
	TArray<AActor*> Objects2;
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		if (GS->CurrentPlayTime <= 0||bIsFinish)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle);
			FinishGame();
		}

		GS->CurrentPlayTime--;
		GS->OnRep_UpdateTime();


		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionTrash::StaticClass(), Objects);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionDecal::StaticClass(), Objects2);
		int32 Time = GS->PlayTime - GS->CurrentPlayTime;
		if (GS->CurrentPlayTime < HintTime)
		{
			Draw = !Draw;
			for (const auto& Object : Objects)
			{
				AInteractionTrash* Trash = Cast<AInteractionTrash>(Object);
				if (Trash)
				{
					GS->S2A_DrawOutline(Trash, Draw);
				}
			}
			for (const auto& Object : Objects2)
			{
				AInteractionDecal* Decal = Cast<AInteractionDecal>(Object);
				if(Decal)
				{
					GS->S2A_DecalDrawOutline(Decal, Draw);
				}
			}
		}
		if (EventTimeIndex < EventTime.Num() && Time == EventTime[EventTimeIndex])
		{
			for (FEntityPowerLevel Entity : Entities)
			{
				CanSpawnEntity(Entity, Time);
			}
			//if (CurrentPowerLevel < 5)
			//{
			//	for (int i = 0; i < 100; i++)
			//	{
			//		for (FEntityPowerLevel Entity : Entities)
			//		{
			//			if (CurrentPowerLevel > 5)
			//				break;
			//			CanSpawnEntity(Entity, Time);
			//		}
			//	}
			//}
			EventTimeIndex++;
		}
	}
}

void AInGameGM::FinishGame()
{
	if (bIsFinish)
	{
		return;
	}

	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		bIsFinish = true;
		GS->UpdatePlayerState();
		GS->UpdateFee();
		GS->UpdateStar();
		GS->S2A_FinishGame(OutTruckCamera);
	}
}

void AInGameGM::OnLevelLoaded(UWorld* InWorld)
{
	if (InWorld)
	{
		AInGameGS* GS = GetGameState<AInGameGS>();
		if (GS)
		{
			GS->PlayerNum = GS->PlayerArray.Num();
		}
	}
}





