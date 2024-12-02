// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Drop.h"
#include "BasicGhost.h"
#include "AIController.h"
#include "BasicInteractionObject.h"
#include "InteractionPickUpObject.h"
#include "BehaviorTree\BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Drop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		ABasicGhost* Ghost = AIC->GetPawn<ABasicGhost>();
		if (Ghost)
		{
			AInteractionPickUpObject* Object = Cast<AInteractionPickUpObject>(Ghost->Target);
			if (Object)
			{
				Object->DropObject(Ghost);
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}