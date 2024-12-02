// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRunAwayLocation.h"
#include "EngineUtils.h"
#include "RunAwayPoint.h"
#include "InGameGM.h"
#include "BasicGhost.h"
#include "AIController.h"


EBTNodeResult::Type UBTTask_GetRunAwayLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = GetWorld();
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	TArray<ARunAwayPoint> RunAwayPoints;
	AInGameGM* GM = Cast < AInGameGM>(World->GetAuthGameMode());
	if(AIC)
	{
		if(GM)
		{
			ABasicGhost* Ghost = Cast<ABasicGhost>(AIC->GetPawn());
			if (Ghost)
			{
				if (GM->RunAwayPoints.Num() >= 1)
				{
					int RandomIndex = FMath::RandRange(0, GM->RunAwayPoints.Num() - 1);
					Ghost->MoveLocation = GM->RunAwayPoints[RandomIndex]->GetActorLocation();
				}
				else
				{
					Ghost->MoveLocation = Ghost->GetActorLocation();
				}
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
