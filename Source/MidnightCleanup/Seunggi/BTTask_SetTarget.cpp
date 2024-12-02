// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetTarget.h"
#include "InteractionPickUpObject.h"
#include "InteractionSwitch.h"
#include "InteractionDoor.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "BasicGhost.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		ABasicGhost* Ghost = AIC->GetPawn<ABasicGhost>();
		int DetectedObjectNum = Ghost->DetectedObject.Num();
		if (DetectedObjectNum > 0)
		{
			int TargetIndex = FMath::RandRange(0, DetectedObjectNum - 1);
			Ghost->Target = Ghost->DetectedObject[TargetIndex];
			FName name = Ghost->Target->GetFName();
			
			AIC->GetBlackboardComponent()->SetValueAsVector(TEXT("Target"), Ghost->Target->GetActorLocation());
			AIC->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetObject"), Ghost->Target);
			Ghost->MoveLocation = Ghost->Target->GetActorLocation();
			return EBTNodeResult::Succeeded;
		}
		
		

	}
	return EBTNodeResult::Failed;
}

