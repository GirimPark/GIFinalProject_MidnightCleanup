// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GhostSpawnPoint.h"
#include "RunAwayPoint.h"
#include "InGameGM.generated.h"

UENUM(BlueprintType)
enum class EEntityType : uint8 {
	Event = 0 UMETA(DisplayName = "Event"),
	Ghost = 1  UMETA(DisplayName = "Ghost"),

};

USTRUCT(BlueprintType)
struct FEntityPowerLevel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityType EntityType=EEntityType::Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Entity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PowerLevel=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPoint;

};
UCLASS()
class MIDNIGHTCLEANUP_API AInGameGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEntityPowerLevel> Entities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABasicGhost> TrashGhost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABasicGhost> LightGhost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABasicGhost> DoorGhost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AInteractionBox> TrashBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrashGhostSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightGhostSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DoorGhostSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HintTime=180;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MaxGhostNum;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MaxPowerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxActiveEntityNum;


	int32 CurrentPowerLevel;

	int32 CurrentActiveEntityNum;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> EventTime;

	int8 EventTimeIndex = 0;

	TArray<APointBase*> RunAwayPoints;
	
	TArray<APawn*> SpawnAI;

	TArray<class ATextureDecal*> SpawnDecal;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACleanProgressMonitor> CleanProgressMonitorClass;
	TObjectPtr<class ACleanProgressMonitor> CleanProgressMonitor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ALimitTimeMonitor> LimitTimesMonitorClass;
	TObjectPtr<class ALimitTimeMonitor> LimitTimeMonitor;

	
	bool bIsFinish=false;

	TArray<FString> UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACameraActor* OutTruckCamera;

	int32 Index = 0;

	bool Draw = true;

public:
	AInGameGM();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	void ChangeLevel(FString InLevelName);

	void CanSpawnEntity(FEntityPowerLevel Object, int32 InTime);

	UFUNCTION(Server, Reliable)
	void SpawnAICharacter(TSubclassOf<ABasicGhost> Ghost , AGhostSpawnPoint* SpawnPoint);
	void SpawnAICharacter_Implementation(TSubclassOf<ABasicGhost> Ghost, AGhostSpawnPoint* SpawnPoint);

	UFUNCTION(Server, Reliable)
	void SpawnBox(TSubclassOf<class AInteractionBox> Box, AObjectSpawnPoint* SpawnPoint);
	void SpawnBox_Implementation(TSubclassOf<class AInteractionBox> Box, AObjectSpawnPoint* SpawnPoint);

	UFUNCTION(Server, Reliable)
	void C2S_SpawnCleanProgressMonitor(TSubclassOf<class ACleanProgressMonitor> Monitor, FTransform Transform);
	void C2S_SpawnCleanProgressMonitor_Implementation(TSubclassOf<class ACleanProgressMonitor> Monitor, FTransform Transform);

	UFUNCTION(Server, Reliable)
	void C2S_SpawnLimitTimeMonitor(TSubclassOf<class ALimitTimeMonitor> Monitor, FTransform Transform);
	void C2S_SpawnLimitTimeMonitor_Implementation(TSubclassOf<class ALimitTimeMonitor> Monitor, FTransform Transform);

	void OpenInitGame();

	void InitCountObject();
	void CountObject();

	UFUNCTION(Server, Reliable)
	void SpawnEntity(TSubclassOf<AActor> Entity, AGhostSpawnPoint* SpawnPoint);
	void SpawnEntity_Implementation(TSubclassOf<AActor> Entity, AGhostSpawnPoint* SpawnPoint);
	
	int SelectEntity(TArray<AActor*> Entity);

	void UpdatePowerLevel(TSubclassOf<AActor>Entity);

	void DecreaseTime();

	void FinishGame();

	void OnLevelLoaded(UWorld* InWorld);
};
