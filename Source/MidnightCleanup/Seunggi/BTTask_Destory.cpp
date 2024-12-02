// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Destory.h"
#include "InGameGM.h"

#include "AIController.h"


EBTNodeResult::Type UBTTask_Destory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		TArray<APawn*>& arr= Cast<AInGameGM>(GetWorld()->GetAuthGameMode())->SpawnAI;
	
		arr.Remove(AIC->GetPawn());
		GetWorld()->DestroyActor(AIC->GetPawn());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
